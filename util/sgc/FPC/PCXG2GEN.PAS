program PCXGrsToGen;
    uses Graph13h,PCXUnit,Utils;
    const
        MAX_TILECOUNT = 40*28;
    type
        RawTile=array[0..7,0..7] of byte;
		GenTile=array[0..7,0..3] of byte;
    var Img:PRawImage;
        MaxTiles:integer;
        T:text;
        Original,Optimized:array[0..Pred(MAX_TILECOUNT)] of RawTile;
        Frequencies,Similars:array[0..Pred(MAX_TILECOUNT)] of integer;
        OptimCount:word;
        BkgMap:array[0..27,0..39] of word;
        CurrTolerance:word;
    procedure ReadTiles;
        var i,j,k:longint;
            X,Y:longint;
        begin
            for k:=0 to Pred(MAX_TILECOUNT) do
                begin
                    for i:=0 to 7 do
                        for j:=0 to 7 do
                            begin
                                X:=((k mod 40)*8)+j;
                                Y:=((k div 40)*8)+i;
                                Original[k][i,j]:=Img^.Data[(Y*320)+X] div 16;
                            end;
                end;
        end;
    procedure InitPalette;
        var i:byte;
            Pal:Palette;
        begin
            for i:=0 to 15 do
                begin
                    Pal[i,0]:=i*2;
                    Pal[i,1]:=i*3;
                    Pal[i,2]:=i*3+16;
                end;
            Pal[16,0]:=63;
            Pal[16,1]:=63;
            Pal[16,2]:=63;
            PutPal(Pal);
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
            for i:=0 to 27 do
                for j:=0 to 39 do
                    DisplayTile(8*j,8*i,Original[(i*40)+j]);
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
    function CalcInterpolatedDifference(var Tile1,Tile2:RawTile):longint;
        var Diff:longint;
            i,j:longint;
            Tile3,Tile4:RawTile;
        begin
            Move(Tile1,Tile3,SizeOf(Tile1));
            Move(Tile2,Tile4,SizeOf(Tile2));
            for i:=1 to 6 do
                for j:=1 to 6 do
                    begin
                        Tile3[i,j]:=(Tile1[i-1,j-1]+
                                     Tile1[i+1,j-1]+
                                     Tile1[i+1,j+1]+
                                     Tile1[i-1,j+1]) div 4;
                        Tile4[i,j]:=(Tile2[i-1,j-1]+
                                     Tile2[i+1,j-1]+
                                     Tile2[i+1,j+1]+
                                     Tile2[i-1,j+1]) div 4;
                    end;
            for i:=1 to 6 do
                begin
                    Tile3[0,i]:=(Tile1[0,i-1]+
                                 Tile1[1,i-1]+
                                 Tile1[1,i+1]+
                                 Tile1[0,i+1]) div 4;
                    Tile3[7,i]:=(Tile1[6,i-1]+
                                 Tile1[7,i-1]+
                                 Tile1[7,i+1]+
                                 Tile1[6,i+1]) div 4;
                    Tile3[i,0]:=(Tile1[i-1,0]+
                                 Tile1[i+1,0]+
                                 Tile1[i+1,1]+
                                 Tile1[i-1,1]) div 4;
                    Tile3[i,7]:=(Tile1[i-1,6]+
                                 Tile1[i+1,6]+
                                 Tile1[i+1,7]+
                                 Tile1[i-1,7]) div 4;
                    //
                    Tile4[0,i]:=(Tile2[0,i-1]+
                                 Tile2[1,i-1]+
                                 Tile2[1,i+1]+
                                 Tile2[0,i+1]) div 4;
                    Tile4[7,i]:=(Tile2[6,i-1]+
                                 Tile2[7,i+1]+
                                 Tile2[7,i+1]+
                                 Tile2[6,i+1]) div 4;
                    Tile4[i,0]:=(Tile2[i-1,0]+
                                 Tile2[i+1,0]+
                                 Tile2[i+1,1]+
                                 Tile2[i-1,1]) div 4;
                    Tile4[i,7]:=(Tile2[i-1,6]+
                                 Tile2[i+1,6]+
                                 Tile2[i+1,7]+
                                 Tile2[i-1,7]) div 4;
                end;
            //
            Tile3[0,0]:=(Tile1[0,0]+
                         Tile1[0,1]+
                         Tile1[1,1]+
                         Tile1[1,0]) div 4;
            Tile3[0,7]:=(Tile1[0,7]+
                         Tile1[0,6]+
                         Tile1[1,6]+
                         Tile1[1,7]) div 4;
            Tile3[7,7]:=(Tile1[7,7]+
                         Tile1[7,6]+
                         Tile1[6,6]+
                         Tile1[6,7]) div 4;
            Tile3[7,0]:=(Tile1[7,0]+
                         Tile1[7,1]+
                         Tile1[6,1]+
                         Tile1[6,0]) div 4;
            Tile4[0,0]:=(Tile2[0,0]+
                         Tile2[0,1]+
                         Tile2[1,1]+
                         Tile2[1,0]) div 4;
            Tile4[0,7]:=(Tile2[0,7]+
                         Tile2[0,6]+
                         Tile2[1,6]+
                         Tile2[1,7]) div 4;
            Tile4[7,7]:=(Tile2[7,7]+
                         Tile2[7,6]+
                         Tile2[6,6]+
                         Tile2[6,7]) div 4;
            Tile4[7,0]:=(Tile2[7,0]+
                         Tile2[7,1]+
                         Tile2[6,1]+
                         Tile2[6,0]) div 4;
            //
            Diff:=0;
            for i:=0 to 7 do
                for j:=0 to 7 do
                    Inc(Diff,Abs(Tile3[i,j]-Tile4[i,j]));
            CalcInterpolatedDifference:=Diff;
        end;
    function GetSimilarTile(var Tile:RawTile;Tolerance:longint):integer;
        var i:integer;
        begin
            i:=0;
            while (i < OptimCount) and (CalcDifference(Tile,Optimized[i]) > Tolerance) do
                Inc(i);
            if i = OptimCount then
                begin
                    Move(Tile,Optimized[i],SizeOf(Tile));
                    Inc(OptimCount);
                end;
            GetSimilarTile:=i;
        end;
    function GetSimilarTileFreq(var Tile:RawTile;TileNum:word;Tolerance:longint):integer;
        var i,j:integer;
            Diff:longint;
            MaxDiff:longint;
            Selected:integer;
        begin
            i:=0;
            MaxDiff:=Tolerance+1;
            Selected:=OptimCount;
            while i < OptimCount do
                begin
//                    Diff:=CalcInterpolatedDifference(Tile,Optimized[i]);
                    Diff:=CalcDifference(Tile,Optimized[i]);
//                    Diff:=Diff*(10+(3*Frequencies[TileNum])) div 10;
                    if Diff < MaxDiff then
                        begin
                            MaxDiff:=Diff;
                            Selected:=i;
                        end;
                    Inc(i);
                end;
            if Selected = OptimCount then
                begin
                    Move(Tile,Optimized[Selected],SizeOf(Tile));
                    Inc(OptimCount);
                end
            else
                begin
                    for i:=0 to 7 do
                        for j:=0 to 7 do
                            begin
                                Optimized[Selected][i,j]:=
                                        (Optimized[Selected][i,j]*3+
                                         Tile[i,j]) div 4;
                            end;
                end;
            GetSimilarTileFreq:=Selected;
        end;
    procedure InitFrequencies;
        var i:integer;
        begin
            OptimCount:=0;
            for i:=0 to Pred(MAX_TILECOUNT) do
                Frequencies[i]:=0;
            for i:=0 to Pred(MAX_TILECOUNT) do
                Similars[i]:=GetSimilarTile(Original[i],0);
            for i:=0 to Pred(MAX_TILECOUNT) do
                Inc(Frequencies[Similars[i]]);
            for i:=0 to Pred(MAX_TILECOUNT) do
                BkgMap[i div 40,i mod 40]:=Similars[i];
        end;
    procedure Optimize(Tolerance:longint);
        var i,j:integer;
        begin
            OptimCount:=0;
            for i:=0 to 27 do
                for j:=0 to 39 do
                    BkgMap[i,j]:=GetSimilarTileFreq(Original[(i*40)+j],Similars[(i*40)+j],Tolerance);
        end;
    procedure DisplayOptimized;
        var i,j:byte;
        begin
            for i:=0 to 27 do
                for j:=0 to 39 do
                    DisplayTile(8*j,8*i,Optimized[BkgMap[i,j]]);
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
        var i,j:longint;
            S:string;
        begin
        	S:=ParamStr(3);
            if Pos('\',S) <> 0 then
                Delete(S,1,Pos('\',S));
            if Pos('.',S) <> 0 then
                Delete(S,Pos('.',S),Length(S));
            S:='_'+S;
        	for i:=1 to Length(S) do
        		begin
        			if S[i] in ['.','-'] then
        				S[i]:='_';
        		end;

        	Writeln(T,#09,'.globl ',S,'_tiles');
        	Writeln(T,S,'_tiles:');
        	for i:=0 to Pred(OptimCount) do
        		begin
        			if i <> 0 then
        				begin
        					Writeln(T);
        					Writeln(T,'* --------------------');
        				end;
        			WriteTile(Optimized[i]);
        		end;
            Writeln(T);
        	Writeln(T,#09,'.globl ',S,'_tiles_end');
        	Writeln(T,S,'_tiles_end:');
        
            Writeln(T);
        	Writeln(T,#09,'.globl ',S,'_map');
        	Writeln(T,S,'_map:');
            for i:=0 to 27 do
                begin
        			if i <> 0 then
        				begin
        					Writeln(T);
        					Writeln(T,'* --------------------');
        				end;
                    for j:=0 to 39 do
                        begin
                            if (j mod 10 = 0) then
                                begin
                                    Writeln(T);
                                    Write(T,#09,'dc.w ');
                                end;
                            if (j mod 10 <> 0) then
                                Write(T,',');
    						Write(T,'$',HexWord(BkgMap[i,j]));
                        end;
                end;
            Writeln(T);
        end;
    procedure OutFileBin;
        var F:file;
            i,j:word;
            Gen:GenTile;
        procedure SwaB(var W:word);
            begin
                W:=((W shr 8) and $FF) or ((W shl 8) and $FF00);
            end;
        begin
            Assign(F,ChangeExt(ParamStr(3),'TIL'));
            Rewrite(F,1);
            for i:=0 to Pred(OptimCount) do
                begin
                    Raw2Gen(Optimized[i],Gen);
                    BlockWrite(F,Gen,SizeOf(Gen));
                end;
            Close(F);

            Assign(F,ChangeExt(ParamStr(3),'MAP'));
            Rewrite(F,1);
            for i:=0 to 27 do
                for j:=0 to 39 do
                    SwaB(BkgMap[i,j]);
            BlockWrite(F,BkgMap,SizeOf(BkgMap));
            Close(F);
        end;
Begin
    if ParamCount < 3 then
        begin
            Writeln('PCXG2GEN v1.0 by Haroldo de Oliveira Pinheiro');
            Writeln('USAGE: PCXG2GEN <max tiles> <grayscale.pcx> <outfile>');
            Halt(1);
        end;

    MaxTiles:=ReadNumber(ParamStr(1));
    if MaxTiles = 0 then
        begin
            Writeln('MaxTiles must be greater than zero.');
            Halt(2);
        end;

    AllocReadPCX(Img,ParamStr(2));
    ReadTiles;

    InitGraph13h;
    LoadFont('FONT.BIN');
    InitPalette;
    DisplayOriginal;
    Buffer2Screen(ScrBuf^);
    InitFrequencies;
    DisplayOptimized;
    OutText(0,0,16,IntToStr(OptimCount));
    Buffer2Screen(ScrBuf^);
    CurrTolerance:=0;
//    OptimCount:=MaxTiles+1;
    while OptimCount > MaxTiles do
        begin
            Optimize(CurrTolerance);
            DisplayOptimized;
            OutText(0,0,16,IntToStr(OptimCount));
            Buffer2Screen(ScrBuf^);
            Inc(CurrTolerance);
        end;
//    Readln;
    for MaxTiles:=0 to 70 do
        begin
            WaitVSyncStart;
        end;
    CloseGraph13h;

{
    Assign(T,ParamStr(3));
    Rewrite(T);
    OutFile;
    Close(T);
}
    OutFileBin;
End.