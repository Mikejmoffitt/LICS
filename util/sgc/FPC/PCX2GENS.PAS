program PCXToGenSpr;
    uses Graph13h,PCXUnit,Utils;
    const
        MAX_TILECOUNT = 2048;
    type
        RawTile=array[0..7,0..7] of byte;
		GenTile=array[0..7,0..3] of byte;
    var Img:PRawImage;
        MaxTiles:integer;
        T:text;
        Original:array[0..Pred(MAX_TILECOUNT)] of RawTile;
        SpriteCount:word;
        BkgMap:array[0..MAX_TILECOUNT] of word;
        Pal:Palette;
        Width,Height:word;
    procedure ReadTiles;
        var i,j,k,l,m,n:longint;
            X,Y:longint;
            nX,nY:longint;
        begin
            Width:=Img^.Width div 8;
            Height:=Img^.Height div 8;
            n:=0;
            for k:=0 to Pred(SpriteCount) do
                begin
{                    for l:=0 to 3 do
                        for m:=0 to 3 do}
                    for m:=0 to 3 do
                        for l:=0 to 3 do // O mega drive organiza sprites por coluna/linha
                            begin
                                nX:=(k mod 9)*33 + (m*8) + 14;
                                nY:=(k div 9)*33 + (l*8) + 9;
                                for i:=0 to 7 do
                                    for j:=0 to 7 do
                                        begin
                                            X:=nX+j;
                                            Y:=nY+i;
                                            Original[n][i,j]:=Img^.Data[(Y*Img^.Width)+X] and $0F;
                                        end;
                                Inc(n);
                            end;
                end;
        end;
    procedure DisplayTile(X,Y:integer;var Tile:RawTile);
        var i,j:byte;
        begin
            for i:=0 to 7 do
                for j:=0 to 7 do
                    PutPixel(X+j,Y+i,Tile[i,j]);
        end;
    procedure DisplayOriginal;
        var i:longint;
        begin
            for i:=0 to Pred(SpriteCount*16) do
                DisplayTile((i mod 40)*8,(i div 40)*8,Original[i]);
        end;
    procedure DisplaySprites;
        var i,j:longint;
            k:longint;
            X,Y:longint;
        begin
            k:=0;
            X:=0;
            Y:=0;
            while k < (SpriteCount*16) do
                begin
                    for i:=0 to 3 do
                        for j:=0 to 3 do
                            begin
//                                DisplayTile((8*j)+X,(8*i)+Y,Original[k]);
                                // O mega drive organiza os sprites por c/l
                                DisplayTile((8*i)+X,(8*j)+Y,Original[k]);
                                inc(k);
                            end;
                    Inc(X,32);
                    if X > 320 then
                        begin
                            X:=0;
                            Inc(Y,32);
                        end;
                end;
        end;
	function HexByte(B:byte):string;
		const
			HexChar:array[$00..$0F] of char=('0','1','2','3',
											 '4','5','6','7',
											 '8','9','A','B',
											 'C','D','E','F');
		begin
			HexByte:=HexChar[B shr 4]+HexChar[B and $0F];
		end;
	function HexWord(W:word):string;
		begin
			HexWord:=HexByte(Hi(W))+HexByte(Lo(W));
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
    procedure OutFile;
		const
			HexChar:array[$00..$0F] of char=('0','1','2','3',
											 '4','5','6','7',
											 '8','9','A','B',
											 'C','D','E','F');
        var F:file;
            i,j:word;
            Gen:GenTile;
            S,S2:string;
            T:text;
        procedure SwaB(var W:word);
            begin
                W:=((W shr 8) and $FF) or ((W shl 8) and $FF00);
            end;
        begin
        	S:=ParamStr(2);
            while Pos('\',S) <> 0 do
                Delete(S,1,Pos('\',S));
            if Pos('.',S) <> 0 then
                Delete(S,Pos('.',S),Length(S));
            S:='_'+S;
        	for i:=1 to Length(S) do
        		begin
        			if S[i] in ['.','-'] then
        				S[i]:='_';
        		end;
            S2:='';
        	for i:=1 to Length(S) do
                S2:=S2+UpCase(S[i]);

            Assign(F,ChangeExt(ParamStr(2),'TIL'));
            Rewrite(F,1);
            for i:=0 to Pred(SpriteCount*16) do
                begin
                    Raw2Gen(Original[i],Gen);
                    BlockWrite(F,Gen,SizeOf(Gen));
                end;
            Close(F);

            Assign(T,ChangeExt(ParamStr(2),'H'));
            Rewrite(T);
            Writeln(T,'uint ',S,'_pal[16]={');
            for i:=0 to 15 do
                begin
                    if i <> 0 then
                        Writeln(T,',');
                    Write(T,'0x');
                    Write(T,HexChar[(Pal[i,2] shr 2) and $0E]);
                    Write(T,HexChar[(Pal[i,1] shr 2) and $0E]);
                    Write(T,HexChar[(Pal[i,0] shr 2) and $0E]);
                end;
            Writeln(T);
            Writeln(T,'};');
            Close(T);
        end;
Begin
    if ParamCount < 3 then
        begin
			Writeln('PCX2GENS v1.0 by Haroldo de Oliveira Pinheiro');
			Writeln('USAGE: PCXG2GENS <image.pcx> <outfile> <spritecount>');
            Halt(1);
        end;

    SpriteCount:=ReadNumber(ParamStr(3));

    AllocReadPCX(Img,ParamStr(1));
    GetPCXPalette(Pal,ParamStr(1));
//    ReadTiles;

    InitGraph13h;
    LoadFont('FONT.BIN');
    PutPal(Pal);
    DisplayRawImageClip(0,0,Img^);
    Buffer2Screen(ScrBuf^);
    for MaxTiles:=0 to 70 do
        begin
            WaitVSyncStart;
        end;

    ReadTiles;
    ClearBuffer(ScrBuf^);
//    DisplayOriginal;
    DisplaySprites;
    Buffer2Screen(ScrBuf^);
    for MaxTiles:=0 to 70 do
        begin
            WaitVSyncStart;
        end;
    CloseGraph13h;

    OutFile;
End.