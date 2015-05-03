unit Graph13h;
{Rotinas gr ficas diversas}
{programado por Haroldo de Oliveira Pinheiro}
{VersÆo 32 bits}
interface
    {$ASMMODE INTEL}
    {$PACKRECORDS 1}
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
    procedure DisplayRawImageStretch(X1,Y1,X2,Y2:integer;var Img:RawImage);
	procedure CutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:RawImage);
	procedure AllocCutRawImage(var Src:RawImage;X1,Y1,X2,Y2:integer;var Dest:PRawImage);
    procedure MirrorRawImage(var Src:RawImage;var Dest:RawImage);
    procedure AllocMirrorRawImage(var Src:RawImage;var Dest:PRawImage);
    procedure TexturizeBackground(X,Y:integer;var Img:RawImage);
    procedure SliceRawImage(var Src:RawImage;XOfs,YOfs:integer;
                            TileW,TileH,CountX,CountY:word;
                            XSpacing,YSpacing:byte;
                            FirstTile,NumTiles:word;
                            var Dest:array of PRawImage);

    procedure PutPal(var Pal);
	procedure ReIndexColors(var Img:RawImage;var Pal:Palette;Colors,NewIndex:byte);
	procedure MakeGradPal(var Pal:Palette;StartColor:byte;StartRGB:Vector;EndColor:byte;EndRGB:Vector);

	procedure Buffer2Screen(var Buf:ScreenBuffer);
	procedure ClearBuffer(var Buf:ScreenBuffer);
	procedure ClearBufferLines(var Buf:ScreenBuffer;FirstLine,LineCount:longint);

	procedure LoadFont(FileName:string);
	procedure PutChar(X,Y:longint;Color:byte;chr:char);
	procedure OutText(X,Y:integer;Color:byte;S:string);
	procedure OutTextFmt(X,Y:integer;Color:byte;S:string);

	procedure HLine(X,Y:longint;Len:longint;Color:byte);
	procedure VLine(X,Y:longint;Len:longint;Color:byte);
    procedure PutPixel(X,Y:longint;Color:byte);

	procedure InitStars(Mode:string;StarCount:byte;var Stars:VectorArray);
	procedure MoveStars(Mode:string;StarCount:byte;Speed:integer;var Stars:VectorArray);
	procedure DrawStars(Mode:string;StarCount:byte;StartColor,EndColor:byte;var Stars:VectorArray);

    procedure WaitVsyncStart;
implementation
	procedure InterpolateVector(var Dest:Vector;V1,V2:Vector;I,Max:longint);
		begin
			Dest.X:=((Longint(V1.X)*(Max-I))+(Longint(V2.X)*I)) div Max;
			Dest.Y:=((Longint(V1.Y)*(Max-I))+(Longint(V2.Y)*I)) div Max;
			Dest.Z:=((Longint(V1.Z)*(Max-I))+(Longint(V2.Z)*I)) div Max;
		end;

	procedure InitGraph13h;assembler;
		asm
			mov eax, 13h
			int 10h
		end;
	procedure CloseGraph13h;assembler;
		asm
			mov eax, 03h
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
			FreeMem(Img,Img^.Width*Img^.Height+4);
		end;
	procedure DisplayRawImage(X,Y:integer;var Img:RawImage);
		var i,j:word;
		begin
			for i:=0 to Pred(Img.Width) do
				for j:=0 to Pred(Img.Height) do
					Mem[$A0000+((j+Y)*320)+i+X]:=Img.Data[(j*Img.Width)+i];
		end;
	procedure DisplayRawImageClip(X,Y:integer;var Img:RawImage);
		var AftrSkip:longint;
			Lines2Skip,Lines2Draw:longint;
			Cols2Skip,Cols2Draw,Cols2Ignr:longint;
			StartAddr:longint;
			ImgOfs:longint;
			XCount:longint;
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
						mov edi, [ScrBuf]
						add edi, StartAddr
						mov esi, [Img]
						add esi, 4
						add esi, ImgOfs
						mov edx, Cols2Skip
						mov ebx, Lines2Draw
					  @RowLoop:
						mov ecx, Cols2Draw
					  @ColLoop:
						lodsb
						or al, al
						jz @SkipPixel
						mov [edi], al
					  @SkipPixel:
						inc edi
						loop @ColLoop
						add esi, edx
						add edi, AftrSkip
						dec ebx
						or  ebx,ebx
						jnz @RowLoop
					end;
				end;
		end;
    procedure DisplayRawImageStretch(X1,Y1,X2,Y2:integer;var Img:RawImage);
        var Tmp:integer;
            H,V:longint;
            Hi,Vi:longint;
            Ho:longint;
            XCnt,YCnt:integer;
            i,j:integer;
            ScrAddr:longint;
            YAddr:longint;
            Color:byte;
        begin
            if X1 > X2 then
                begin
                    Tmp:=X1;
                    X1:=X2;
                    X2:=Tmp;
                end;
            if Y1 > Y2 then
                begin
                    Tmp:=Y1;
                    Y1:=Y2;
                    Y2:=Tmp;
                end;

            XCnt:=(X2 - X1)+1;
            YCnt:=(Y2 - Y1)+1;

            Hi:=(Img.Width shl 16) div XCnt;
            Vi:=(Img.Height shl 16) div YCnt;

            Ho:=0;
            if X2 > MaxX then
                begin
                    X2:=MaxX;
                    XCnt:=(X2 - X1)+1;
                end;
            if X1 < MinX then
                begin
                    Ho:=Hi*(MinX-X1);
                    X1:=MinX;
                    XCnt:=(X2 - X1)+1;
                end;            

            V:=0;
            if Y2 > MaxY then
                begin
                    Y2:=MaxY;
                    YCnt:=(Y2 - Y1)+1;
                end;
            if Y1 < MinY then
                begin
                    V:=Vi*(MinY-Y1);
                    Y1:=MinY;
                    YCnt:=(Y2 - Y1)+1;
                end;            

            ScrAddr:=(Y1*320)+X1;

            for i:=0 to YCnt do
                begin
                    YAddr:=(V shr 16)*Img.Width;
                    H:=Ho;
                    for j:=0 to Pred(XCnt) do
                        begin
                            Color:=Img.Data[YAddr+(H shr 16)];
                            if Color <> 0 then
                                ScrBuf^[0,ScrAddr]:=Color;
                            Inc(H,Hi);
                            Inc(ScrAddr);
                        end;
                    Inc(V,Vi);
                    Inc(ScrAddr,320-XCnt);
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
			GetMem(Dest,(X2-X1+1)*(Y2-Y1+1)+4);
			CutRawImage(Src,X1,Y1,X2,Y2,Dest^);
		end;
    procedure MirrorRawImage(var Src:RawImage;var Dest:RawImage);
        var i,j:integer;
        begin
            Dest.Width:=Src.Width;
            Dest.Height:=Src.Height;
			for i:=0 to Pred(Dest.Width) do
				for j:=0 to Pred(Dest.Height) do
					Dest.Data[(j*Dest.Width)+i]:=Src.Data[(j*Src.Width)+Pred(Dest.Width-i)];
        end;
    procedure AllocMirrorRawImage(var Src:RawImage;var Dest:PRawImage);
        begin
			GetMem(Dest,Src.Width*Src.Height+4);
            MirrorRawImage(Src,Dest^);
        end;

    procedure TexturizeBackground(X,Y:integer;var Img:RawImage);
        var i,j:word;
            XCount,YCount:word;
        begin
            if X < 0 then
                X:=-(Abs(X) mod Img.Width)
            else if X > 0 then
                X:=(X mod Img.Width)-Img.Width;
            //
            if Y < 0 then
                Y:=-(Abs(Y) mod Img.Height)
            else if Y > 0 then
                Y:=(Y mod Img.Height)-Img.Height;
            //
            XCount:=(Pred(MaxX-(MinX+X)) div Img.Width)+2;
            YCount:=(Pred(MaxY-(MinY+Y)) div Img.Height)+2;
            //
            for i:=0 to Pred(XCount) do
                for j:=0 to Pred(YCount) do
                    begin
                        DisplayRawImageClip((i*Img.Width)+MinX+X,
                                            (j*Img.Height)+MinY+Y,
                                            Img);
                    end;
        end;
    procedure SliceRawImage(var Src:RawImage;XOfs,YOfs:integer;
                            TileW,TileH,CountX,CountY:word;
                            XSpacing,YSpacing:byte;
                            FirstTile,NumTiles:word;
                            var Dest:array of PRawImage);
        var i:word;
            XInc,YInc:word;
        begin
            XInc:=TileW+XSpacing;
            YInc:=TileH+YSpacing;
            for i:=0 to Pred(NumTiles) do
                begin
                    AllocCutRawImage(Src,
                                     ((i mod CountX)*XInc)+XOfs,   
                                     ((i div CountX)*YInc)+YOfs,   
                                     ((i mod CountX)*XInc)+XOfs+TileW-1,   
                                     ((i div CountX)*YInc)+YOfs+TileH-1,   
                                     Dest[i+FirstTile]);
                end;
        end;

    procedure PutPal(var Pal);assembler;
        asm
            mov esi,[Pal];
            mov dx, $3C8;
            mov al, 0;
            out dx, al;
            mov ecx, 256*3;
            inc dx;
          @ColorLoop:
            mov al, [esi];
            out dx, al;
            inc esi;
            loop @ColorLoop;
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
            cld
            mov dx, es
			mov esi, [Buf]
            mov ax, fs
            mov es, ax
			mov eax, $A0000
			mov edi, eax
			mov ecx, 16000
            rep movsd
            mov es, dx
		end;
	procedure ClearBuffer(var Buf:ScreenBuffer);assembler;
		asm
            cld
			mov edi, [Buf]
			xor eax, eax
			mov ecx, 16000
			rep stosd
		end;
	procedure ClearBufferLines(var Buf:ScreenBuffer;FirstLine,LineCount:longint);assembler;
		asm
            cld
			mov edi, [Buf]
            mov eax, FirstLine
            shl eax, 6
            add edi, eax
            shl eax, 2
            add edi, eax
			mov eax, LineCount
            shl eax, 4
            mov ecx, eax
            shl eax, 2
            add ecx, eax
			xor eax, eax
			rep stosd
		end;

	type
		Font8x8Mono=array[0..7] of byte;
		CharSet=array[0..258] of Font8x8Mono; //Coloca uma sobra extra no fim
	var Font:^CharSet;
	procedure LoadFont(FileName:string);
		var F:file;
		begin
			Assign(F,FileName);
			Reset(F,1);
//			BlockRead(F,Font^,SizeOf(Font^));
			BlockRead(F,Font^,FileSize(F));
			Close(F);
		end;
	procedure PutChar(X,Y:longint;Color:byte;chr:char);assembler;
		asm
			cld;
			{C lculo do endere‡o inicial do buffer}
			mov edi, [ScrBuf]
			mov eax, Y
			shl eax, 6
			add edi, eax
			shl eax, 2
			add edi, eax
			add edi, X
			{C lculo do endere‡o inicial da fonte}
			xor eax, eax
			mov esi, [Font]
			mov al, chr
			shl eax, 3
			add esi, eax
			add esi, 4
			{loop principal}
			mov ah, Color
			mov ecx, 8
		  @RowLoop:
			mov ebx, ecx
			mov ecx, 8
			lodsb
		  @ColLoop:
			shl al, 1
			jnc @SkipPixel
			mov [edi], ah
		  @SkipPixel:
			inc edi
			loop @ColLoop
			mov ecx, ebx
			add edi, (320-8)
			loop @RowLoop
		end;
	procedure OutText(X,Y:integer;Color:byte;S:string);
		var i:byte;
		begin
            if S <> '' then
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

	procedure HLine(X,Y:longint;Len:longint;Color:byte);
		begin
			if (Y >= MinY) and (Y <= MaxY) and (X <= MaxX) and (X+Len > MinX) then
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
								mov edi, [ScrBuf]
								mov eax, Y
								shl eax, 6
								add edi, eax
								shl eax, 2
								add edi, eax
								add edi, X
								mov ecx, Len
								mov al, Color
								rep stosb
							end;
						end;
				end;
		end;
	procedure VLine(X,Y:longint;Len:longint;Color:byte);
		begin
			if (X >= MinX) and (X <= MaxX) and (Y <= MaxY) and (Y+Len > MinY) then
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
								mov edi, [ScrBuf]
								mov eax, Y
								shl eax, 6
								add edi, eax
								shl eax, 2
								add edi, eax
								add edi, X
								mov ecx, Len
								mov al, Color
							  @RowLoop:
								mov [edi], al
								add edi, 320
								loop @RowLoop
							end;
						end;
				end;
		end;
	procedure PutPixel(X,Y:longint;Color:byte);
		begin
			if (X >= MinX) and (X <= MaxX) and (Y <= MaxY) and (Y >= MinY) then
				begin
					asm
						mov edi, [ScrBuf]
						mov eax, Y
						shl eax, 6
						add edi, eax
						shl eax, 2
						add edi, eax
						add edi, X
						mov al, Color
						mov [edi], al
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
			for i:=0 to Pred(StarCount) do
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
									nX:=(Longint(X)*256) div Z + MinX;
									nY:=(Longint(Y)*256) div Z + MinY;
								end;
							if (ModeCod <> 2) and (nX >= MinX) and (nX <= MaxX) and (nY >= MinY) and (nY <= MaxY) then
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
			for i:=0 to Pred(StarCount) do
				begin
					with Stars[i] do
						begin
							if Z <> 0 then
								begin
									if DirCod = 0 then
										begin
											nX:=(Longint(X)*256) div Z + MinX;
											nY:=(Longint(Y)*256) div Z + MinY;
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

    procedure WaitVsyncStart; assembler;
        asm
            mov dx,$03DA
          @WaitNotVsync:
            in  al,dx
            test al,08h
            jnz @WaitNotVsync
         @WaitVsync:
            in  al,dx
            test al,08h
            jz @WaitVsync
        end;
Begin
	New(ScrBuf);
	New(Font);
End.