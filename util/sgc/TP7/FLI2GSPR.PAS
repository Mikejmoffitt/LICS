program Fli2GenesisSprite;
	uses Crt,X_Const,Utils,FliUnit;
	type
		RawTile=array[0..7,0..7] of byte;
		GenTile=array[0..7,0..3] of byte;
		RawSprite=array[0..15] of RawTile;
		GenSprite=array[0..15] of GenTile;
	var i,j,k,l:word;
		Pal:Palette;
		PalConvData:array[0..7,0..7,0..7] of byte;
		SprTilW,SprTilH:byte;
		SprPixW,SprPixH:word;
		UsePalette:boolean;
		TrR,TrG,TrB:byte;
		UseTran:boolean;
		ColorsToUse:byte;
		StartIndex:byte;
		InputFile,OutputFile:string;
		CurrFrame:longint;
		F:file;
	procedure ReadPalette(PalName:string);
		var T:text;
			S:string;
			i:byte;
		begin
			Assign(T,PalName);
			Reset(T);
			i:=0;
			while (i <> 15) and not Eof(T) do
				begin
					Readln(T,S);
					Pal[i,0]:=HexToInt(GetToken(S)) shl 4;
					Pal[i,1]:=HexToInt(GetToken(S)) shl 4;
					Pal[i,2]:=HexToInt(GetToken(S)) shl 4;
					Inc(i);
				end;
			Close(T);
		end;
	function ColorDiff(R1,G1,B1,R2,G2,B2:byte):longint;
		begin
			ColorDiff:=Sqr(Longint(R1)-Longint(R2))+
					   Sqr(Longint(G1)-Longint(G2))+
					   Sqr(Longint(B1)-Longint(B2));
		end;
	function BestMatch(R,G,B:byte):byte;
		var Current,Best:longint;
			Index:byte;
			i:byte;
		begin
			Best:=$7FFFFFFF;
			Index:=0;
			for i:=0 to 15 do
				begin
					Current:=ColorDiff(R,G,B,
									   Pal[i,0],
									   Pal[i,1],
									   Pal[i,2]);
					if Current < Best then
						begin
							Best:=Current;
							Index:=i;
						end;
				end;
			BestMatch:=Index;
		end;
	function BestMatch2(R,G,B:byte):byte;
		var Current,Best:longint;
			Index:byte;
			i:byte;
		begin
			Best:=$7FFFFFFF;
			Index:=0;
			for i:=StartIndex to Pred(StartIndex+ColorsToUse) do
				begin
					Current:=ColorDiff(R,G,B,
									   Pal[i,0],
									   Pal[i,1],
									   Pal[i,2]);
					if Current < Best then
						begin
							Best:=Current;
							Index:=i;
						end;
				end;
			BestMatch2:=Index;
		end;

	procedure ShowTitle;
		begin
			Writeln('FLI to Genesis Sprite v1.0');
			Writeln('by Haroldo de Oliveira Pinheiro');
		end;
	procedure ShowUsage;
		begin
			ShowTitle;
			Writeln('USAGE: FLI2GSPR [<option> [<option>...]] <input.fli> [<output.til>]');
			Writeln('Available options are:');
			Writeln(' -?           Shows this help screen');
			Writeln(' -W <value>   Sprite width, in tiles (each tile is 8 pixels wide).');
			Writeln(' -H <value>   Sprite height, in tiles (each tile is 8 pixels tall)');
			Writeln(' -P <file>    Specify palette file');
			Writeln(' -T <r><g><b> Specify color to use as transparency (hex)');
			Writeln(' -C <value>   Specify number of colors to use');
			Writeln(' -I <value>   Specify starting color index');
		end;
	procedure ShowParams;
		begin
			ShowTitle;
			Writeln;
			Writeln('Parameters:');
			Writeln('Sprite is ',SprTilW,'x',SprTilH,' tiles (',SprPixW,'x',SprPixH,' pixels)');
			if UsePalette then
				Writeln('Choosing colors by similarity')
			else
				Writeln('Choosing colors by index');
			if UseTran then
				Writeln('Using (',TrR,',',TrG,',',TrB,') as transparent color');
			Writeln('Using ',ColorsToUse,' colors, starting from ',StartIndex);
			Writeln('Input file: ',InputFile);
			Writeln('Output file: ',OutputFile);
		end;
	procedure ReadCommandLine;
		var i : byte;
			Parm : string;
		begin
			if ParamCount = 0 then
				begin
					ShowUsage;
					Halt(1);
				end;

			i := 1;
			while i <= ParamCount do
				begin
					Parm := ParamStr(i);
					if (Parm[1] = '-') and (Length(Parm) > 1) then
						case UpCase(Parm[2]) of
							'?':begin
									ShowUsage;
									Halt(0);
								end;
							'W':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											SprTilW := ReadNumber(Parm);
											SprPixW := SprTilW * 8;
										end;
								end;
							'H':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											SprTilH := ReadNumber(Parm);
											SprPixH := SprTilH * 8;
										end;
								end;
							'P':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											ReadPalette(Parm);
											UsePalette := True;
										end;
								end;
							'T':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											TrR := HexToInt(Copy(Parm,1,2));
											TrG := HexToInt(Copy(Parm,3,2));
											TrB := HexToInt(Copy(Parm,5,2));
											UseTran := True;
										end;
								end;
							'C':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											ColorsToUse := ReadNumber(Parm);
										end;
								end;
							'I':begin
									Inc(i);
									if i <= ParamCount then
										begin
											Parm := ParamStr(i);
											StartIndex := ReadNumber(Parm);
										end;
								end;
						end
					else
						begin
							if InputFile = '' then
								InputFile := ParamStr(i)
							else
								OutputFile := ParamStr(i);
						end;
					Inc(i);
				end;
			if InputFile = '' then
				begin
					ShowUsage;
					Halt(2);
				end;
			if OutputFile = '' then
				OutputFile := ChangeExt(InputFile,'TIL');
		end;
	procedure SetDefaults;
		begin
			SprTilW:=1;
			SprTilH:=1;
			SprPixW:=8;
			SprPixH:=8;
			UsePalette:=False;
			TrR:=0;
			TrG:=0;
			TrB:=0;
			UseTran:=False;
			ColorsToUse:=16;
			StartIndex:=0;
			InputFile:='';
			OutputFile:='';
		end;

	procedure DescribeSpriteAssembly;
		begin
			Writeln('Sprite will be composed of:');
			Writeln(' ',(SprTilW div 4)*(SprTilH div 4),' 4x4 Genesis sprite(s)');
			if (SprTilW mod 4) <> 0 then {Sobras na horizontal}
				Writeln(' ',(SprTilH div 4),' ',(SprTilW mod 4),'x4 Genesis sprite(s)');
			if (SprTilH mod 4) <> 0 then {Sobras na vertical}
				Writeln(' ',(SprTilW div 4),' 4x',(SprTilH mod 4),' Genesis sprite(s)');
			if ((SprTilW mod 4) <> 0) and ((SprTilH mod 4) <> 0) then {Sobras na horizontal E na vertical}
				Writeln(' 1 ',(SprTilW mod 4),'x',(SprTilH mod 4),' Genesis sprite');
		end;

	procedure PreparePalConvData;
		var i,j,k:byte;
		begin
			if StartIndex+ColorsToUse > 16 then
				ColorsToUse := 16 - StartIndex;

			for i := 0 to 7 do
				for j := 0 to 7 do
					for k := 0 to 7 do
						PalConvData[i,j,k]:=BestMatch2(i shl 5, j shl 5, k shl 5);
		end;

	procedure CaptureRawTile(X,Y:integer;var Tile:RawTile);
		var i, j:byte;
		begin
			for i:=0 to 7 do
				for j := 0 to 7 do
					Tile[i,j]:=FramePix^[Y+i,X+j];
		end;
	procedure CaptureSprite(X,Y:integer;W,H:byte;var Tiles:array of RawTile);
		var i, j, k:byte;
		begin
			k := 0;
			for j:=0 to Pred(W) do
				for i:=0 to Pred(H) do {Os sprites do Mega Drive sÆo armazenados em Coluna/Linha}
					begin
						CaptureRawTile(X+(Word(j)*8),Y+(Word(i)*8),Tiles[k]);
						Inc(k);
					end;
		end;
	procedure Raw2Gen(var Src:RawTile;var Dst:GenTile);
		var i,j,k:byte;
		begin
			for i:=0 to 7 do
				for j:=0 to 3 do
					begin
						Dst[i,j]:=Src[i,(j*2)] and $0F;
						Dst[i,j]:=Dst[i,j] shl 4;
						Dst[i,j]:=Dst[i,j] or Src[i,(j*2)+1] and $0F;
					end;
		end;
	function ConvPixel(Color:byte):byte;
		var Temp: byte;
		begin
			Temp := 255;

			if UseTran then
				if (FramePal[Color,0] = TrR) and
				   (FramePal[Color,1] = TrG) and
				   (FramePal[Color,0] = TrB) then
					Temp := 0;

			if Temp = 255 then
				begin
					If UsePalette Then
						Temp := PalConvData[FramePal[Color,0] shr 3,
											FramePal[Color,1] shr 3,
											FramePal[Color,2] shr 3]
					Else
						Temp := (Color mod ColorsToUse) + StartIndex;
				end;

			ConvPixel := Temp and $0F;
		end;
	procedure ConvRaw2Gen(var Src:RawTile;var Dst:GenTile);
		var i,j,k:byte;
		begin
			for i:=0 to 7 do
				for j:=0 to 3 do
					begin
						Dst[i,j]:=ConvPixel(Src[i,(j*2)]);
						Dst[i,j]:=Dst[i,j] shl 4;
						Dst[i,j]:=Dst[i,j] or ConvPixel(Src[i,(j*2)+1]);
					end;
		end;
	procedure ProcessFrame;
		var OfsX,OfsY:integer;
			TotW,TotH:integer;
			SprW,SprH:byte;
			RawTileSet:array[0..Pred(4*4)] of RawTile;
			GenTileSet:array[0..Pred(4*4)] of GenTile;
			i:byte;
		begin
			OfsX:=160 - (SprPixW div 2);
			OfsY:=100 - (SprPixH div 2);

			TotW:=SprTilW;
			TotH:=SprTilH;

			while TotH > 0 do
				begin
					if TotH > 4 then
						SprH:=4
					else
						SprH:=TotH;

					OfsX:=160 - (SprPixW div 2);
					TotW:=SprTilW;

					while TotW > 0 do
						begin
							if TotW > 4 then
								SprW:=4
							else
								SprW:=TotW;

							CaptureSprite(OfsX,OfsY,SprW,SprH,RawTileSet);
							for i:=0 to Pred(SprW*SprH) do
								ConvRaw2Gen(RawTileSet[i],GenTileSet[i]);
							BlockWrite(F,GenTileSet,(SprW*SprH)*SizeOf(GenTile));

							Inc(OfsX,32);
							Dec(TotW,4);
						end;

					Inc(OfsY,32);
					Dec(TotH,4);
				end;
		end;
Begin
	SetDefaults;
	ReadCommandLine;
	ShowParams;

	DescribeSpriteAssembly;

	Writeln('Starting conversion...');

	if UsePalette then
		PreparePalConvData;

	InitFli(InputFile);
	CurrFrame := 1;

	Assign(F,OutputFile);
	Rewrite(F,1);

	while FliNextFrame do
		begin
			Write('Frame ',CurrFrame:5,' of ',FliFrameCount:5,#13);
			ProcessFrame;
			Inc(CurrFrame);
		end;
	Writeln;

	Close(F);

	Writeln('Done');
End.