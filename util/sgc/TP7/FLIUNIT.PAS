{Unidade FliPlay: Unidade para ler e mostrar arquivos FLI}
{Programado por Haroldo de Oliveira Pinheiro}
unit FliUnit;
interface
	uses X_Const;
	type
		FramePic=array[0..199,0..319] of byte;
	var FramePix:^FramePic;
		FramePal:Palette;
		FLIFrameCount:word;
	procedure PlayFLI(FileName:string);
	procedure InitFLI(FileName:string);
	function  FLINextFrame:boolean;
	procedure DoneFLI;
implementation
	uses X_Main,X_Pal;
	type
		FliHeader = record
						Length:longint;
						Magic:word;		   {AF11 para FLI padr∆o}
						Frames:word;	   {M†ximo 4000}
						Width:word;		   {320 para FLI padr∆o}
						Height:word;	   {200 para FLI padr∆o}
						Depht:word;		   {8 para FLI padr∆o (256 cores)}
						Flags:word;		   {Precisa ser 0}
						Speed:word;		   {Pausa entre os quadros de animaá∆o}
						Next:longint;	   {Precisa ser 0}
						Frit:longint;	   {??? (Precisa ser zero)}
						Expand:array[0..101] of byte;{Futura expans∆o}
					end;
		FrameHeader=record
						Size:longint;
						Magic:word;	{Sempre F1FA}
						Chunks:word;{N£mero de pedaáos que comp‰em o quadro}
						Expand:array[0..7] of byte;{Futura expans∆o}
					end;
		ChunkHeader=record
						Size:longint;
						ChnkType:word;
					end;
	const{Tipos de "Chunks"}
		CHNK_COLOR=11;{Palheta}
		CHNK_LC=12;{Linha}
		CHNK_BLACK=13;{Cor 0 para a tela toda}
		CHNK_BRUN=15;{RLE}
		CHNK_COPY=16;{64000 bytes brutos}
	type
		ChunkBuffer=array[0..65534] of byte;
	var ChunkBuf:^ChunkBuffer;
		BufPos:word;
		Header:FliHeader;
		CurFrameHeader:FrameHeader;
		CurChunkHeader:ChunkHeader;
	procedure GetData(var Dest;Size:word);
		begin
			Move(ChunkBuf^[BufPos],Dest,Size);
			Inc(BufPos,Size);
		end;
	procedure FLI_Color;
		var PacketCount:word;
			ToSkip,ToChange:byte;
			i,j:word;
			ColorNum:byte;
		begin
			GetData(PacketCount,2);
			ColorNum:=0;
			for i:=0 to Pred(PacketCount) do
				begin
					GetData(ToSkip,1);
					Inc(ColorNum,ToSkip);
					GetData(ToChange,1);
					if ToChange=0 then
						GetData(FramePal,256*3)
					else
						begin
							for j:=0 to Pred(ToChange) do
								begin
									GetData(FramePal[ColorNum],3);
									Inc(ColorNum);
								end;
						end;
				end;
		end;
	procedure FLI_LC;
		var	LinesToSkip:word;
			LinesToChange:word;
			PacketCount:byte;
			PixToSkip:byte;
			PixToChange:shortint;
			X,Y:word;
			i,j:word;
			Color:byte;
		begin
			X:=0;
			Y:=0;
			GetData(LinesToSkip,2);
			Inc(Y, LinesToSkip);
			GetData(LinesToChange,2);
			for i:=0 to Pred(LinesToChange) do
				begin
					GetData(PacketCount,1);
					if PacketCount<>0 then
						for j:=0 to Pred(PacketCount) do
							begin
								GetData(PixToSkip,1);
								Inc(X,PixToSkip);
								GetData(PixToChange,1);
								if PixToChange > 0 then
									GetData(FramePix^[Y,X],PixToChange)
								else
									begin
										GetData(Color,1);
										FillChar(FramePix^[Y,X],-PixToChange,Color);
									end;
								Inc(X,Abs(PixToChange));
							end;
					X:=0;
					Inc(Y);
				end;
		end;
	procedure FLI_Black;
		begin
			FillChar(FramePix^,64000,0);
		end;
	procedure FLI_BRun;
		var	PacketCount:byte;
			Size_Count:shortint;
			Color:integer;
			i,j:word;
			X:word;
		begin
			for i:=0 to 199 do
				begin
					X:=0;
					GetData(PacketCount,1);
					for j:=0 to Pred(PacketCount) do
						begin
							GetData(Size_Count,1);
							if Size_Count > 0 then
								begin
									GetData(Color,1);
									FillChar(FramePix^[i,X],Size_Count,Color);
								end
							else
								begin
									GetData(FramePix^[i,X],-Size_Count);
								end;
							Inc(X,Abs(Size_Count));
						end;
				end;
		end;
	procedure FLI_Copy;
		begin
			GetData(FramePix^,64000);
		end;
	procedure Buf2Scr;
		var i,j:word;
		begin
			X_Put_Pal_Raw(FramePal,256,0,True);
			for i:=0 to 199 do
				for j:=0 to 319 do
					PutPixel(j,i,FramePix^[i,j]);
		end;
	procedure PlayFLI(FileName:string);
		var F:file;
			i,j:word;
		begin
			{$I-}
			Assign(F,FileName);
			Reset(F,1);
{			if IOResult <> 0 then
				begin
					Writeln;
					WriteLn('FliPlay v1.0 by Haroldo de Oliveira Pinheiro');
					Writeln('   Usage: FLIPLAY <Filename>');
					Halt(1);
				end;}
			{$I+}
			BlockRead(F,Header,SizeOf(Header));
			New(ChunkBuf);
			New(FramePix);
			X_Set_Mode(X_Mode_320x200,320);
			for i:=0 to Pred(Header.Frames) do
				begin
					BlockRead(F,CurFrameHeader,SizeOf(CurFrameHeader));
					if CurFrameHeader.Magic <> $F1FA then
						begin
							X_Text_Mode;
							Writeln('Erro no cabeáalho do quadro de animaá∆o: ',
									CurFrameHeader.Magic);
							Close(F);
							Halt(1);
						end;
					BlockRead(F,ChunkBuf^,CurFrameHeader.Size-16);
					BufPos:=0;
					if CurFrameHeader.Chunks<>0 then
						begin
							for j:=0 to Pred(CurFrameHeader.Chunks) do
								begin
									GetData(CurChunkHeader,SizeOf(CurChunkHeader));
									case CurChunkHeader.ChnkType of
										CHNK_COLOR:
											begin
												FLI_Color;
											end;
										CHNK_LC:
											begin
												FLI_LC;
											end;
										CHNK_BLACK:
											begin
												FLI_Black;
											end;
										CHNK_BRUN:
											begin
												FLI_BRun;
											end;
										CHNK_COPY:
											begin
												FLI_Copy;
											end;
									end;
								end;
						end;
					Buf2Scr;
				end;
			Dispose(ChunkBuf);
			Dispose(FramePix);
			X_Text_Mode;
			Close(F);
		end;
	var FLIFile:file;
		CurrFrame:word;
	procedure InitFLI(FileName:string);
		begin
			Assign(FLIFile,FileName);
			Reset(FLIFile,1);
			BlockRead(FLIFile,Header,SizeOf(Header));
			New(ChunkBuf);
			New(FramePix);
			CurrFrame:=0;
			FLIFrameCount:=Header.Frames;
		end;
	function  FLINextFrame:boolean;
		var j:word;
		begin
			if CurrFrame < Header.Frames then
				begin
					Inc(CurrFrame);
					FLINextFrame:=true;
					BlockRead(FLIFile,CurFrameHeader,SizeOf(CurFrameHeader));
					if CurFrameHeader.Magic <> $F1FA then
						begin
							X_Text_Mode;
							Writeln('Erro no cabeáalho do quadro de animaá∆o: ',
									CurFrameHeader.Magic);
							Close(FLIFile);
							Halt(1);
						end;
					BlockRead(FLIFile,ChunkBuf^,CurFrameHeader.Size-16);
					BufPos:=0;
					if CurFrameHeader.Chunks<>0 then
						begin
							for j:=0 to Pred(CurFrameHeader.Chunks) do
								begin
									GetData(CurChunkHeader,SizeOf(CurChunkHeader));
									case CurChunkHeader.ChnkType of
										CHNK_COLOR:
											begin
												FLI_Color;
											end;
										CHNK_LC:
											begin
												FLI_LC;
											end;
										CHNK_BLACK:
											begin
												FLI_Black;
											end;
										CHNK_BRUN:
											begin
												FLI_BRun;
											end;
										CHNK_COPY:
											begin
												FLI_Copy;
											end;
									end;
								end;
						end;
				end
			else
				begin
					FLINextFrame:=false;
				end;
		end;
	procedure DoneFLI;
		begin
			Dispose(ChunkBuf);
			Dispose(FramePix);
			Close(FLIFile);
		end;
End.