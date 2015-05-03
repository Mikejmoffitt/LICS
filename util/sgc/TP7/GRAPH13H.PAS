unit Graph13h;
{Rotinas gr ficas diversas}
{programado por Haroldo de Oliveira Pinheiro}
interface
	const
		MinX:integer=0;
		MaxX:integer=319;
		MinY:integer=0;
		MaxY:integer=199;
	type
		Palette=array[0..255,0..2] of byte;
		Vector= record
					X,Y,Z:integer
				end;
		VectorArray=array[0..255] of Vector;
		PRawImage=^RawImage;
		RawImage=record
					Width,Height:word;
					Data:array[0..65530] of byte;
				 end;
		ScreenBuffer=array[0..199,0..319] of byte;
	var ScrBuf:^ScreenBuffer;

	procedure InitGraph13h;
	procedure CloseGraph13h;

	procedure AllocRawImage(var Img:PRawImage;Width,Height:word);
	procedure FreeRawImage(var Img:PRawImage);
	procedure DisplayRawImage(X,Y:integer;var Img:RawImage);
	procedure DisplayRawImageClip(X,Y:integer;var Img:RawImage);
	procedure CutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:RawImage);
	procedure AllocCutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:PRawImage);

	procedure ReIndexColors(var Img:RawImage;var Pal:Palette;Colors,NewIndex:byte);
	procedure MakeGradPal(var Pal:Palette;StartColor:byte;StartRGB:Vector;EndColor:byte;EndRGB:Vector);

	procedure Buffer2Screen(var Buf:ScreenBuffer);
	procedure ClearBuffer(var Buf:ScreenBuffer);

	procedure LoadFont(FileName:string);
	procedure LoadDefaultFont;
	procedure PutChar(X,Y:integer;Color:byte;chr:char);
	procedure OutText(X,Y:integer;Color:byte;S:string);
	procedure OutTextFmt(X,Y:integer;Color:byte;S:string);

	procedure HLine(X,Y:integer;Len:integer;Color:byte);
	procedure VLine(X,Y:integer;Len:integer;Color:byte);

	procedure InitStars(Mode:string;StarCount:byte;var Stars:VectorArray);
	procedure MoveStars(Mode:string;StarCount:byte;Speed:integer;var Stars:VectorArray);
	procedure DrawStars(Mode:string;StarCount:byte;StartColor,EndColor:byte;var Stars:VectorArray);
implementation
	procedure InterpolateVector(var Dest:Vector;V1,V2:Vector;I,Max:longint);
		begin
			Dest.X:=((Longint(V1.X)*(Max-I))+(Longint(V2.X)*I)) div Max;
			Dest.Y:=((Longint(V1.Y)*(Max-I))+(Longint(V2.Y)*I)) div Max;
			Dest.Z:=((Longint(V1.Z)*(Max-I))+(Longint(V2.Z)*I)) div Max;
		end;

	procedure InitGraph13h;assembler;
		asm
			mov ax, 13h
			int 10h
		end;
	procedure CloseGraph13h;assembler;
		asm
			mov ax, 03h
			int 10h
		end;

	procedure AllocRawImage(var Img:PRawImage;Width,Height:word);
		begin
			GetMem(Img,Width*Height+4);
			Img^.Width:=Width;
			Img^.Height:=Height;
		end;
	procedure FreeRawImage(var Img:PRawImage);
		begin
			GetMem(Img,Img^.Width*Img^.Height+4);
		end;
	procedure DisplayRawImage(X,Y:integer;var Img:RawImage);
		var i,j:word;
		begin
			for i:=0 to Pred(Img.Width) do
				for j:=0 to Pred(Img.Height) do
					Mem[$A000:((j+Y)*320)+i+X]:=Img.Data[(j*Img.Width)+i];
		end;
	procedure DisplayRawImageClip(X,Y:integer;var Img:RawImage);
		var AftrSkip:word;
			Lines2Skip,Lines2Draw:word;
			Cols2Skip,Cols2Draw,Cols2Ignr:word;
			StartAddr:word;
			ImgOfs:word;
			XCount:word;
		begin
			XCount:=MaxX-MinX+1;
			Dec(X,MinX);
			if (X > -Img.Width) and (X < XCount) and (Y > MinY-Img.Height) and (Y <= MaxY) then
				begin
					Cols2Draw:=Img.Width;
					Cols2Skip:=0;
					Cols2Ignr:=0;
					if X < 0 then
						begin
							Inc(Cols2Draw,X); {X ‚ negativo}
							Cols2Skip:=-X;
							X:=0;
						end;
					if X+Cols2Draw > XCount then
						begin
							Cols2Draw:=XCount-X;
							Cols2Ignr:=Img.Width-Cols2Draw;
						end;
					AftrSkip:=320-Cols2Draw;

					Lines2Draw:=Img.Height;
					Lines2Skip:=0;
					if Y < MinY then
						begin
							Inc(Lines2Draw,Y-MinY); {Y-MinY ‚ negativo}
							Lines2Skip:=MinY-Y;
							Y:=MinY;
						end;
					if Y+Lines2Draw >= MaxY  then
						begin
							Lines2Draw:=MaxY-Y+1;
						end;

					Inc(X,MinX);
					StartAddr:=(Y*320)+X;
					ImgOfs:=(Lines2Skip*Img.Width)+Cols2Skip;
					Inc(Cols2Skip,Cols2Ignr);
					asm
						cld
						push ds
						les di, [ScrBuf] { aconselh vel carregar todos os dados ANTES de mudar ds}
						add di, StartAddr
						lds si, [Img]
						add si, 4
						add si, ImgOfs
{						mov ax, 0A000h
						mov es, ax
						mov di, StartAddr} {A antiga versÆo desenhava direto para a tela}
						mov dx, Cols2Skip
						mov bx, Lines2Draw
					  @RowLoop:
						mov cx, Cols2Draw
					  @ColLoop:
						lodsb
						or al, al
						jz @SkipPixel
						mov es:[di], al
					  @SkipPixel:
						inc di
						loop @ColLoop
						add si, dx
						add di, AftrSkip
						dec bx
						or  bx,bx
						jnz @RowLoop
						pop ds
					end;
				end;
		end;
	procedure CutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:RawImage);
		var i,j:integer;
		begin
			Dest.Width:=(X2-X1)+1;
			Dest.Height:=(Y2-Y1)+1;
			for i:=0 to X2-X1 do
				for j:=0 to Y2-Y1 do
					Dest.Data[(j*Dest.Width)+i]:=Src.Data[((j+Y1)*Src.Width)+i+X1];
		end;
	procedure AllocCutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:PRawImage);
		begin
			GetMem(Dest,(X2-X1+1)*(Y2-Y2+1));
			CutRawImage(Src,X1,Y1,X2,Y2,Dest^);
		end;

	procedure ReIndexColors(var Img:RawImage;var Pal:Palette;Colors,NewIndex:byte);
		var i,j:word;
			TempPal:Palette;
		begin
			FillChar(TempPal,SizeOf(TempPal),0);
			Move(Pal[0],TempPal[NewIndex],3*Colors);
			Pal:=TempPal;
			for i:=0 to Pred(Img.Width) do
				for j:=0 to Pred(Img.Height) do
					begin
						If Img.Data[(j*Img.Width)+i] >= Colors then
							Img.Data[(j*Img.Width)+i]:=0
						else
							Inc(Img.Data[(j*Img.Width)+i],NewIndex);
					end;
		end;
	procedure MakeGradPal(var Pal:Palette;StartColor:byte;StartRGB:Vector;EndColor:byte;EndRGB:Vector);
		var i:byte;
			NewRGB:Vector;
		begin
			for i:=0 to (EndColor-StartColor) do
				begin
					InterPolateVector(NewRGB,StartRGB,EndRGB,i,EndColor-StartColor);
					Pal[i+StartColor,0]:=NewRGB.X shr 2;
					Pal[i+StartColor,1]:=NewRGB.Y shr 2;
					Pal[i+StartColor,2]:=NewRGB.Z shr 2;
				end;
		end;

	procedure Buffer2Screen(var Buf:ScreenBuffer);assembler;
		asm
			push ds
			lds si, [Buf]
			mov ax, $A000
			mov es, ax
			xor di, di
			mov cx, 16000
			db $66; rep movsw {rep movsd}
			pop ds
		end;
	procedure ClearBuffer(var Buf:ScreenBuffer);assembler;
		asm
			les di, [Buf]
			db $66; xor ax, ax {xor eax, eax}
			mov cx, 16000
			db $66; rep stosw {rep stosd}
		end;

	type
		Font8x8Mono=array[0..7] of byte;
		CharSet=array[char] of Font8x8Mono;
	var Font:^CharSet;
	procedure LoadFont(FileName:string);
		var F:file;
		begin
			Assign(F,FileName);
			Reset(F,1);
			BlockRead(F,Font^,SizeOf(Font^));
			Close(F);
		end;
	{$L FONT.OBJ}
	procedure DefaultFont;external;
	procedure LoadDefaultFont;
		var DefFnt:pointer;
		begin
			DefFnt:=Addr(DefaultFont);
			Move(DefFnt^,Font^,SizeOf(Font^));
		end;
	procedure PutChar(X,Y:integer;Color:byte;chr:char);assembler;
		asm
			cld;
			push ds;
			{C lculo do endere‡o inicial do buffer}
			les di, [ScrBuf]
			mov ax, Y
			shl ax, 6
			add di, ax
			shl ax, 2
			add di, ax
			add di, X
			{C lculo do endere‡o inicial da fonte}
			lds si, [Font]
			mov al, chr
			xor ah, ah
			shl ax, 3
			add si, ax
			add si, 4
			{loop principal}
			mov ah, Color
			mov cx, 8
		  @RowLoop:
			mov bx, cx
			mov cx, 8
			lodsb
		  @ColLoop:
			shl al, 1
			jnc @SkipPixel
			mov es:[di], ah
		  @SkipPixel:
			inc di
			loop @ColLoop
			mov cx, bx
			add di, (320-8)
			loop @RowLoop
			pop ds;
		end;
	procedure OutText(X,Y:integer;Color:byte;S:string);
		var i:byte;
		begin
			if S = '' then
				Exit;
			for i:=0 to Pred(Length(S)) do
				PutChar((i shl 3)+X,Y,Color,S[i+1]);
		end;
	procedure OutTextFmt(X,Y:integer;Color:byte;S:string);
		var i:byte;
			nX,nY:integer;
		begin
			i:=1;
			nX:=0;
			nY:=0;
			while i <= Length(S) do
				begin
					if S[i]='\' then
						begin
							if i < Length(S) then
								Inc(i);
							case S[i] of
								'n':begin
										nX:=0;
										Inc(nY,8);
									end;
							end;
						end
					else
						begin
							PutChar(nX+X,nY+Y,Color,S[i]);
							Inc(nX,8);
						end;
					Inc(i);
				end;
		end;

	procedure HLine(X,Y:integer;Len:integer;Color:byte);
		begin
			if (Y >= MinY) and (Y <= MaxY) and (X <= MaxX) then
				begin
					if X < MinX then
						begin
							Dec(Len,MinX-X);
							X:=MinX;
						end;
					if (Len <> 0) and (X+Len > MaxX) then
						begin
							Len:=MaxX-X+1;
						end;
					if Len <> 0 then
						begin
							asm
								les di, [ScrBuf]
								mov ax, Y
								shl ax, 6
								add di, ax
								shl ax, 2
								add di, ax
								add di, X
								mov cx, Len
								mov al, Color
								rep stosb
							end;
						end;
				end;
		end;
	procedure VLine(X,Y:integer;Len:integer;Color:byte);
		begin
			if (X >= MinX) and (X <= MaxX) and (Y <= MaxY) then
				begin
					if Y < MinY then
						begin
							Dec(Len,MinY-Y);
							Y:=MinY;
						end;
					if (Len <> 0) and (Y+Len > MaxY) then
						begin
							Len:=MaxY-Y+1;
						end;
					if Len <> 0 then
						begin
							asm
								les di, [ScrBuf]
								mov ax, Y
								shl ax, 6
								add di, ax
								shl ax, 2
								add di, ax
								add di, X
								mov cx, Len
								mov al, Color
							  @RowLoop:
								mov es:[di], al
								add di, 320
								loop @RowLoop
							end;
						end;
				end;
		end;

	procedure InitStars(Mode:string;StarCount:byte;var Stars:VectorArray);
		var i:byte;
			CountX,CountY:integer;
		begin
			CountX:=MaxX-MinX+1;
			CountY:=MaxY-MinY+1;
			if Pos('x',Mode) <> 0 then
				begin
					for i:=0 to Pred(StarCount) do
						begin
							Stars[i].Z:=Random(255)+1;
							Stars[i].X:=Longint(Random(CountX))*Stars[i].Z div 256;
							Stars[i].Y:=Longint(Random(CountY))*Stars[i].Z div 256;
						end;
				end
			else if Pos('y',Mode) <> 0 then
				begin
					for i:=0 to Pred(StarCount) do
						begin
							Stars[i].Z:=Random(255)+1;
							Stars[i].X:=Longint(Random(CountX))*Stars[i].Z div 256;
							Stars[i].Y:=Longint(Random(CountY))*Stars[i].Z div 256;
						end;
				end
			else if Pos('z',Mode) <> 0 then
				begin
					for i:=0 to Pred(StarCount) do
						begin
							Stars[i].Z:=Random(255)+1;
							Stars[i].X:=Longint(Random(CountX)-160)*Stars[i].Z div 64;
							Stars[i].Y:=Longint(Random(CountY)-100)*Stars[i].Z div 64;
						end;
				end;
		end;
	procedure MoveStars(Mode:string;StarCount:byte;Speed:integer;var Stars:VectorArray);
		var i:byte;
			ModeCod:byte;
			CountX,CountY:integer;
			nX,nY:integer;
		begin
			CountX:=MaxX-MinX+1;
			CountY:=MaxY-MinY+1;
			if Pos('x',Mode) <> 0 then
				ModeCod:=0
			else if Pos('y',Mode) <> 0 then
				ModeCod:=1
			else if Pos('z',Mode) <> 0 then
				ModeCod:=2;
			for i:=0 to 255 do
				begin
					with Stars[i] do
						begin
							case ModeCod of
								0:X:=X+Speed;
								1:Y:=Y+Speed;
								2:Z:=Z+Speed;
							end;
							if Z <> 0 then
								begin
									nX:=(Longint(X)*256) div Z;
									nY:=(Longint(Y)*256) div Z;
								end;
							if (ModeCod <> 2) and (nX >= MinX) and (nX <= MaxX) and (nY >= MinX) and (nY <= MaxX) then
								begin
								end
							else if (ModeCod = 2) and (Z > 0) and (Z < 256) then
								begin
								end
							else
								begin
									case ModeCod of
										0:begin
											Z:=Random(255)+1;
											if Speed < 0 then
												X:=Longint(Random(20)+MaxX-20)*Z div 256
											else
												X:=Longint(Random(20))*Z div 256;
											Y:=Random(200);
										  end;
										1:begin
											Z:=Random(255)+1;
											X:=Random(320);
											if Speed < 0 then
												Y:=Longint(Random(20)+MaxY-20)*Z div 256
											else
												Y:=Longint(Random(20))*Z div 256;
										  end;
										2:begin
											if Speed < 0 then
												Z:=255
											else
												Z:=0;
											X:=Random(CountX)-160;
											Y:=Random(CountX)-100;
										  end;
									end;
								end;
						end;
			end;
		end;
	procedure DrawStars(Mode:string;StarCount:byte;StartColor,EndColor:byte;var Stars:VectorArray);
		var i:byte;
			ModeCod:byte;
			DirCod:byte;
			nX,nY:integer;
			Color,ColorCount:byte;
		begin
			if Pos('n',Mode) <> 0 then
				ModeCod:=0
			else if Pos('h',Mode) <> 0 then
				ModeCod:=1
			else if Pos('v',Mode) <> 0 then
				ModeCod:=2;
			DirCod:=0;
			if Pos('z',Mode) <> 0 then
				DirCod:=2;
			ColorCount:=EndColor-StartColor;
			for i:=0 to 255 do
				begin
					with Stars[i] do
						begin
							if Z <> 0 then
								begin
									if DirCod = 0 then
										begin
											nX:=(Longint(X)*256) div Z + MinX;
											nY:=(Longint(Y)*256) div Z + MinX;
										end
									else
										begin
											nX:=(Longint(X)*256) div Z+((MinX+MaxX) shr 1);
											nY:=(Longint(Y)*256) div Z+((MinY+MaxY) shr 1);
										end;
								end;
							if (nX >= MinX) and (nX <= MaxX) and (nY >= MinY) and (nY < MaxY) then
								begin
									Color:=EndColor-(Longint(Z)*ColorCount div 255);
									case ModeCod of
										0:ScrBuf^[nY,nX]:=Color;
										1:HLine(nX,nY,16,Color);
										2:VLine(nX,nY,16,Color);
									end;
								end;
						end;
			end;
		end;
Begin
	New(ScrBuf);
	New(Font);
	LoadDefaultFont;
End.