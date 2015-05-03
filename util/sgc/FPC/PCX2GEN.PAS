program PCXGrsToGen;
    uses Graph13h,PCXUnit,Utils;
    const
        MAX_TILECOUNT = 2048;
    type
        RawTile=array[0..7,0..7] of byte;
		GenTile=array[0..7,0..3] of byte;
    var Img:PRawImage;
        MaxTiles:integer;
        T:text;
        Original,Optimized:array[0..Pred(MAX_TILECOUNT)] of RawTile;
        Frequencies,Similars:array[0..Pred(MAX_TILECOUNT)] of integer;
        OptimCount:word;
        BkgMap:array[0..MAX_TILECOUNT] of word;
        CurrTolerance:word;
        Pal:Palette;
        Width,Height:word;
    procedure ReadTiles;
        var i,j,k:longint;
            X,Y:longint;
        begin
            Width:=Img^.Width div 8;
            Height:=Img^.Height div 8;
            for k:=0 to Pred(Width*Height) do
                begin
                    for i:=0 to 7 do
                        for j:=0 to 7 do
                            begin
                                X:=((k mod Width)*8)+j;
                                Y:=((k div Width)*8)+i;
                                Original[k][i,j]:=Img^.Data[(Y*Img^.Width)+X] and $0F;
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
        var i,j:byte;
        begin
            for i:=0 to Pred(Height) do
                for j:=0 to Pred(Width) do
                    DisplayTile(8*j,8*i,Original[(i*Width)+j]);
        end;
    function CalcDifference(var Tile1,Tile2:RawTile):longint;
        var Diff:longint;
            i,j:longint;
        begin
            Diff:=0;
            for i:=0 to 7 do
                for j:=0 to 7 do
                    Inc(Diff,Abs(Tile1[i,j]-Tile2[i,j]));
            CalcDifference:=Diff;
        end;
    function GetSimilarTile(var Tile:RawTile):integer;
        var i:integer;
        begin
            i:=0;
            while (i < OptimCount) and (CalcDifference(Tile,Optimized[i]) > 0) do
                Inc(i);
            if i = OptimCount then
                begin
                    Move(Tile,Optimized[i],SizeOf(Tile));
                    Inc(OptimCount);
                end;
            GetSimilarTile:=i;
        end;
    procedure Optimize;
        var i:word;
        begin
            OptimCount:=0;
            for i:=0 to Pred(Width*Height) do
                BkgMap[i]:=GetSimilarTile(Original[i]);
        end;
    procedure DisplayOptimized;
        var i,j:byte;
        begin
            for i:=0 to Pred(Height) do
                for j:=0 to Pred(Width) do
                    DisplayTile(8*j,8*i,Optimized[BkgMap[i*Width+j]]);
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
	procedure WriteTile(var Raw:RawTile);
		var Gen:GenTile;
			i,j:byte;
		begin
			for i:=0 to 7 do
				for j:=0 to 3 do
					Gen[i,j]:=$00;
			RAW2Gen(Raw,Gen);
			for i:=0 to 7 do
				begin
					Write(T,#09,'dc.l $');
					for j:=0 to 3 do
						Write(T,HexByte(Gen[i,j]));
					Writeln(T);
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
            for i:=0 to Pred(OptimCount) do
                begin
                    Raw2Gen(Optimized[i],Gen);
                    BlockWrite(F,Gen,SizeOf(Gen));
                end;
            Close(F);

            Assign(F,ChangeExt(ParamStr(2),'MAP'));
            Rewrite(F,1);
            for i:=0 to Pred(Width*Height) do
                SwaB(BkgMap[i]);
            BlockWrite(F,BkgMap,Width*Height*2);
            Close(F);

            Assign(T,ChangeExt(ParamStr(2),'H'));
            Rewrite(T);
            Writeln(T,'#define ',S2,'_WIDTH  ',Width:2);
            Writeln(T,'#define ',S2,'_HEIGHT ',Height:2);
            Writeln(T);
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
    if ParamCount < 2 then
        begin
			Writeln('PCX2GEN v1.0 by Haroldo de Oliveira Pinheiro');
			Writeln('USAGE: PCXG2GEN <image.pcx> <outfile>');
            Halt(1);
        end;

    AllocReadPCX(Img,ParamStr(1));
    GetPCXPalette(Pal,ParamStr(1));
    ReadTiles;

    InitGraph13h;
    LoadFont('FONT.BIN');
    PutPal(Pal);
    DisplayOriginal;
    Buffer2Screen(ScrBuf^);
    Writeln(Width,'x',Height);
    Optimize;
    DisplayOptimized;
    OutText(0,0,16,IntToStr(OptimCount));
    Buffer2Screen(ScrBuf^);
    for MaxTiles:=0 to 70 do
        begin
            WaitVSyncStart;
        end;
    CloseGraph13h;

    OutFile;
End.