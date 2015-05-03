program GENED;
	uses Crt,X_Const,X_Pal,Graph13h,G13hTerm,Mouse;
	const
		ProgInfo:string='GENED v1.0 by Haroldo de Oliveira Pinheiro';

		YesNo:array[boolean] of string[3]=('No ','Yes');

		DefaultMouseCursor:array[0..67] of byte=(8,0,8,0,
												 $8,$8,$0,$0,$0,$0,$0,$0,
												 $8,$F,$8,$0,$0,$0,$0,$0,
												 $8,$F,$F,$8,$0,$0,$0,$0,
												 $8,$F,$F,$F,$8,$0,$0,$0,
												 $8,$F,$F,$F,$F,$8,$0,$0,
												 $8,$F,$F,$F,$F,$F,$8,$0,
												 $8,$8,$8,$F,$8,$8,$8,$8,
												 $0,$0,$8,$8,$8,$8,$0,$0);

		TIL_PRI_MASK = $80;
		TIL_PAL_MASK = $60;
		TIL_VFL_MASK = $10;
		TIL_HFL_MASK = $08;
		TIL_SET_MASK = $07;

		TIL_PAL_SHFT = 5;

		COLOR_SEL_X = 72;
		COLOR_SEL_Y = 8;

		PALETTE_SEL_X = 100;
		PALETTE_SEL_Y = 8;

		TILE_EDIT_X = 16;
		TILE_EDIT_Y = 8;

		TILE_CHOOSE_X = 16;
		TILE_CHOOSE_Y = 70;

		MAP_EDIT_X = 152;
		MAP_EDIT_Y = 8;

		ST_BAR_X = 152;
		ST_BAR_Y = 160;

	type
		GenTile=array[0..7,0..3] of byte;
		GenTileSet=array[0..6,0..255] of GenTile;
		GenPalette=array[0..3,0..15] of word;
	var Mouse_X,Mouse_Y:integer;
		Mouse_B:byte;
		Prog_Close:boolean;
		Pal:Palette;
		BlinkCtrl:byte;
		TileSetFile:string;
		TileMapFile:string;
		PaletteFile:string;
		ClipBoard:GenTile;
		TileSet:^GenTileSet;
		CurrTile:^GenTile;
		TileMap:array[0..127,0..127,0..1] of byte;
		GenPal:GenPalette;
		TilEdBuf,TilChosBuf,TilMapBuf:PRawImage;
		CurrTileSet,CurrTileNum,CurrDefPal:byte;
		ForeColor,BackColor:byte;
		MapTop,MapLeft,MapX,MapY:byte;
		VFlip,HFlip:boolean;
		Priority:byte;
		Superimpose:boolean;
	procedure InitMouse;
		var Rst:ResetRec;
		begin
			MReset(Rst);
			if not Rst.Exists then
				begin
					CloseGraph13h;
					Writeln(ProgInfo);
					Writeln('Error: No mouse driver found.');
					Halt(1);
				end;
			MColRange(0,319);
			MRowRange(0,199);
		end;
	procedure ReadMouse;
		var Loc:LocRec;
		begin
			MPos(Loc);
			Mouse_X:=Loc.Column;
			Mouse_Y:=Loc.Row;
			Mouse_B:=Loc.ButtonStatus;
		end;
	procedure DrawMouseCursor;
		var Img:PRawImage;
		begin
			Img:=Addr(DefaultMouseCursor);
			DisplayRawImageClip(Mouse_X,Mouse_Y,Img^);
		end;
	procedure InitPalette;
		var i,j:byte;
		begin
			Pal[ 0,0]:=$00; Pal[ 0,1]:=$00; Pal[ 0,2]:=$00;
			Pal[ 1,0]:=$00; Pal[ 1,1]:=$00; Pal[ 1,2]:=$2F;
			Pal[ 2,0]:=$00; Pal[ 2,1]:=$2F; Pal[ 2,2]:=$00;
			Pal[ 3,0]:=$00; Pal[ 3,1]:=$2F; Pal[ 3,2]:=$2F;
			Pal[ 4,0]:=$2F; Pal[ 4,1]:=$00; Pal[ 4,2]:=$00;
			Pal[ 5,0]:=$2F; Pal[ 5,1]:=$00; Pal[ 5,2]:=$2F;
			Pal[ 6,0]:=$2F; Pal[ 6,1]:=$2F; Pal[ 6,2]:=$00;
			Pal[ 7,0]:=$2F; Pal[ 7,1]:=$2F; Pal[ 7,2]:=$2F;
			Pal[ 8,0]:=$0F; Pal[ 8,1]:=$0F; Pal[ 8,2]:=$0F;
			Pal[ 9,0]:=$0F; Pal[ 9,1]:=$0F; Pal[ 9,2]:=$3F;
			Pal[10,0]:=$0F; Pal[10,1]:=$3F; Pal[10,2]:=$0F;
			Pal[11,0]:=$0F; Pal[11,1]:=$3F; Pal[11,2]:=$3F;
			Pal[12,0]:=$3F; Pal[12,1]:=$0F; Pal[12,2]:=$0F;
			Pal[13,0]:=$3F; Pal[13,1]:=$0F; Pal[13,2]:=$3F;
			Pal[14,0]:=$3F; Pal[14,1]:=$3F; Pal[14,2]:=$0F;
			Pal[15,0]:=$3F; Pal[15,1]:=$3F; Pal[15,2]:=$3F;

			for i:=0 to 3 do
				for j:=0 to 15 do
					begin
{						Pal[(i*4)+j+16,0]:=((3-j) shl 4)+$0F;
						Pal[(i*4)+j+16,1]:=((3-j) shl 4)+$0F;
						Pal[(i*4)+j+16,2]:=((3-j) shl 4)+$07;}
						Pal[(i*16)+j+16,0]:=Pal[j,0];
						Pal[(i*16)+j+16,1]:=Pal[j,1];
						Pal[(i*16)+j+16,2]:=Pal[j,2];
					end;

			X_Put_Pal_RAW(Pal,256,0,false);
		end;
	procedure PutTile(X,Y:integer;var Tile:GenTile;Attr:byte;var Img:RawImage);
		var i,j:byte;
			Tx,Ty,BaseColor:byte;
			Tmp,Color:byte;
		begin
			BaseColor:=(Attr and TIL_PAL_MASK) shr TIL_PAL_SHFT;
			BaseColor:=(BaseColor*16)+16;
			for i:=0 to 7 do
				begin
					Ty:=i;
					if (Attr and TIL_VFL_MASK) <> 0 then
						Ty:=7-i;
					Inc(Ty,Y);
					for j:=0 to 7 do
						begin
							Tx:=j;
							if (Attr and TIL_HFL_MASK) <> 0 then
								Tx:=7-j;
							Tmp:=Tile[i,j shr 1];
							Inc(Tx,X);
							if j and $01 <> 0 then
								Color:=Tmp and $0F
							else
								Color:=(Tmp shr 4) and $0F;
							Inc(Color,BaseColor);
							Img.Data[(Ty*Img.Width)+Tx]:=Color;
						end;
				end;
		end;
	procedure PutTranspTile(X,Y:integer;var Tile:GenTile;Attr:byte;var Img:RawImage);
		var i,j:byte;
			Tx,Ty,BaseColor:byte;
			Tmp,Color:byte;
		begin
			BaseColor:=(Attr and TIL_PAL_MASK) shr TIL_PAL_SHFT;
			BaseColor:=(BaseColor*16)+16;
			for i:=0 to 7 do
				begin
					Ty:=i;
					if (Attr and TIL_VFL_MASK) <> 0 then
						Ty:=7-i;
					Inc(Ty,Y);
					for j:=0 to 7 do
						begin
							Tx:=j;
							if (Attr and TIL_HFL_MASK) <> 0 then
								Tx:=7-j;
							Tmp:=Tile[i,j shr 1];
							if j and $01 <> 0 then
								Color:=Tmp and $0F
							else
								Color:=(Tmp shr 4) and $0F;
							Inc(Tx,X);
							If Color <> 0 Then
								begin
									Inc(Color,BaseColor);
									Img.Data[(Ty*Img.Width)+Tx]:=Color;
								end;
						end;
				end;
		end;
	function GetTilePixel(X,Y:byte;var Tile:GenTile):byte;
		var Tmp:byte;
			Mask:byte;
			Color:byte;
		begin
			X:=X and $07;
			Y:=Y and $07;
			Tmp:=Tile[Y,X shr 1];

			if X and $01 <> 0 then
				Color := Tmp and $0F
			else
				Color := (Tmp shr 4) and $0F;

			GetTilePixel:=Color;
		end;
	procedure SetTilePixel(X,Y,Color:byte;var Tile:GenTile);
		var Tmp:byte;
		begin
			X:=X and $07;
			Y:=Y and $07;
			Color:=Color and $0F;
			Tmp:=Tile[Y,X shr 1];

			if X and $01 <> 0 then
				Color := Color or (Tmp and $F0)
			else
				Color := (Color shl 4) or (Tmp and $0F);

			Tile[Y,X shr 1]:=Color;
		end;
	procedure HFlipTile(var Tile:GenTile);
		var i,j,k:byte;
		procedure FlipByte(var B:byte);
			var B2:byte;
			begin
				B2:=(B and $0F) shl 4;
				B:=(B and $F0) shr 4;
				B:=B or B2;
			end;
		begin
			for i:=0 to 7 do
				begin
					for j:=0 to 3 do
						FlipByte(Tile[i,j]);
					for j:=0 to 1 do
						begin
							k:=Tile[i,j];
							Tile[i,j]:=Tile[i,3-j];
							Tile[i,3-j]:=k;
						end;
				end;
		end;
	procedure VFlipTile(var Tile:GenTile);
		var i,j,k:byte;
		begin
			for i:=0 to 3 do
				for j:=0 to 3 do
					begin
						k:=Tile[i,j];
						Tile[i,j]:=Tile[7-i,j];
						Tile[7-i,j]:=k;
					end;
		end;
	procedure RotateTile(var Tile:GenTile);
		var i,j:byte;
			Aux:GenTile;
		begin
			for i:=0 to 7 do
				for j:=0 to 7 do
					SetTilePixel(7-i,j,GetTilePixel(j,i,Tile),Aux);
			Move(Aux,Tile,SizeOf(Tile));
		end;
	procedure ShiftTile(var Tile:GenTile;X,Y:byte);
		var i,j:byte;
			Aux:GenTile;
		begin
			X:=(8-X) and $07;
			Y:=(8-Y) and $07;

			for i:=0 to 7 do
				for j:=0 to 7 do
					SetTilePixel(j,i,GetTilePixel((j+X) and $07,i,Tile),Aux);

			for i:=0 to 7 do
				for j:=0 to 7 do
					SetTilePixel(j,i,GetTilePixel(j,(i+Y) and $07,Aux),Tile);
		end;
	procedure EncodeGenPal(var Dest:GenPalette);
		function Gen_RGB(R,G,B:word):word;
			begin
				Gen_RGB:=(B shl 1) or (G shl 13) or (R shl 9);
			end;
		var i,j:byte;
		begin
			for i:=0 to 3 do
				for j:=0 to 15 do
					begin
						Dest[i,j]:=Gen_RGB(Pal[(i*16)+j+16,0] shr 3,
										   Pal[(i*16)+j+16,1] shr 3,
										   Pal[(i*16)+j+16,2] shr 3);
					end;
		end;
	procedure DecodeGenPal(var Src:GenPalette);
		function Gen_R(W:word):byte;
			begin
				Gen_R:=(W shr 9) and $07;
			end;
		function Gen_G(W:word):byte;
			begin
				Gen_G:=(W shr 13) and $07;
			end;
		function Gen_B(W:word):byte;
			begin
				Gen_B:=(W shr 1) and $07;
			end;
		var i,j:byte;
		begin
			for i:=0 to 3 do
				for j:=0 to 15 do
					begin
						Pal[(i*16)+j+16,0]:=(Gen_R(Src[i,j]) shl 3);
						Pal[(i*16)+j+16,1]:=(Gen_G(Src[i,j]) shl 3);
						Pal[(i*16)+j+16,2]:=(Gen_B(Src[i,j]) shl 3);
					end;
		end;
	procedure RefreshTilEdBuf;
		var i,j,k,l:byte;
			Color,BaseColor:byte;
		begin
			BaseColor:=(CurrDefPal*16)+16;
			for i:=0 to 7 do
				begin
					for j:=0 to 7 do
						begin
							Color:=GetTilePixel(j,i,CurrTile^);
							Inc(Color,BaseColor);
							for k:=0 to 5 do
								for l:=0 to 5 do
									TilEdBuf^.Data[((i*6+k)*TilEdBuf^.Width)+(j*6+l)]:=Color;
						end;
				end;
		end;
	procedure RefreshTilChosBuf;
		var i,j,k:byte;
		begin
			for i:=0 to 15 do
				for j:=0 to 15 do
					begin
						k:=(i*16)+j;
						PutTile(j*8,i*8,
								TileSet^[CurrTileSet,k],CurrDefPal shl TIL_PAL_SHFT,
								TilChosBuf^);
					end;
		end;
	procedure RefreshTilMapBuf;
		var i,j:word;
			TileNum:byte;
			TSetNum:byte;
			Attr:byte;
		begin
			for i:=0 to 17 do
				for j:=0 to 19 do
					begin
						TileNum:=TileMap[i+MapTop,j+MapLeft,1];
						Attr:=TileMap[i+MapTop,j+MapLeft,0];
						TSetNum:=Attr and TIL_SET_MASK;
						PutTile(j*8,i*8,
								TileSet^[TSetNum,TileNum],Attr,
								TilMapBuf^);

{						if Superimpose then
							begin
								TileNum:=TileMap[(i+MapTop+16) and $1F,j+MapLeft];
								Attr:=AttrMap[(i+MapTop+16) and $1F,j+MapLeft];
								TSetNum:=0;
								if Attr and TIL_BNK_MASK <> 0 then
									TSetNum:=1;
								PutTranspTile(j*8,i*8,
											  TileSet^[TSetNum,TileNum],Attr,
											  TilMapBuf^);
							end;}
					end;
		end;
	procedure RefreshAll;
		begin
			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
        end;
	procedure InitTiles;
		begin
			CurrTileSet:=0;
			CurrTileNum:=0;
			CurrDefPal:=0;

			ForeColor:=3;
			BackColor:=0;

			MapTop:=0;
			MapLeft:=0;
			MapX:=0;
			MapY:=0;

			VFlip:=false;
			HFlip:=false;
			Priority:=0;

			Superimpose:=false;

			FillChar(TileSet^,SizeOf(TileSet^),0);
			CurrTile:=Addr(TileSet^[CurrTileSet,CurrTileNum]);
			FillChar(TileMap,SizeOf(TileMap),0);

			AllocRawImage(TilEdBuf,48,48);
			RefreshTilEdBuf;
			AllocRawImage(TilChosBuf,128,128);
			RefreshTilChosBuf;
			AllocRawImage(TilMapBuf,160,144);
			RefreshTilMapBuf;

			TileSetFile:='';
			TileMapFile:='';
			PaletteFile:='';

		end;
	procedure ReadTileSet(StartTile,Count:word;FileName:string);
		var F:file;
			ToRead:word;
		begin
			FillChar(TileSet^[0,StartTile],SizeOf(GenTile)*Count,0);
			ToRead:=SizeOf(GenTile)*Count;
			Assign(F,FileName);
			Reset(F,1);
			if FileSize(F) < ToRead then
				ToRead:=FileSize(F);
			BlockRead(F,TileSet^[0,StartTile],ToRead);
			Close(F);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
		end;
	procedure WriteTileSet(StartTile,Count:word;FileName:string);
		var F:file;
		begin
			Assign(F,FileName);
			Rewrite(F,1);
			BlockWrite(F,TileSet^[0,StartTile],SizeOf(GenTile)*Count);
			Close(F);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
		end;
	procedure ReadMapFile(X,Y,Width,Height:byte;FileName:string);
		var F:file;
			NumRead:word;
			i:byte;
		begin
			for i:=0 to Pred(Height) do
				FillChar(TileMap[i+Y,X],Width*2,0);

			Assign(F,FileName);
			Reset(F,1);
			for i:=0 to Pred(Height) do
				BlockRead(F,TileMap[i+Y,X],Width*2,NumRead);
			Close(F);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
		end;
	procedure WriteMapFile(X,Y,Width,Height:byte;FileName:string);
		var F:file;
			i:byte;
		begin
			Assign(F,FileName);
			Rewrite(F,1);
			for i:=0 to Pred(Height) do
				BlockWrite(F,TileMap[i+Y,X],Width*2);
			Close(F);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
		end;
	procedure ReadPaletteFile(PalNum:byte;FileName:string);
		var F:file;
			ToRead:word;
		begin
			FillChar(GenPal[PalNum],SizeOf(GenPal[0]),0);
			ToRead:=SizeOf(GenPal[0]);
			Assign(F,FileName);
			Reset(F,1);
			if FileSize(F) < ToRead then
				ToRead:=FileSize(F);
			BlockRead(F,GenPal[PalNum],ToRead);
			Close(F);

			DecodeGenPal(GenPal);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
			X_Put_Pal_RAW(Pal,256,0,false);
		end;
	procedure WritePaletteFile(PalNum:byte;FileName:string);
		var F:file;
		begin
			EncodeGenPal(GenPal);
			Assign(F,FileName);
			Rewrite(F,1);
			BlockWrite(F,GenPal[PalNum],SizeOf(GenPal[0]));
			Close(F);

			RefreshTilEdBuf;
			RefreshTilChosBuf;
			RefreshTilMapBuf;
			X_Put_Pal_RAW(Pal,256,0,false);
		end;
	procedure DrawRectangle(X,Y,W,H:integer;Color:byte);
		begin
			HLine(X,Y,W,Color);
			HLine(X,Y+H-1,W,Color);
			VLine(X,Y,H,Color);
			VLine(X+W-1,Y,H,Color);
		end;
	procedure DrawTileChooseBorder;
		const HexNibble:array[$0..$F] of char=('0','1','2','3',
											   '4','5','6','7',
											   '8','9','A','B',
											   'C','D','E','F');
		var i:byte;
		begin
			for i:=$0 to $F do
				begin
					PutChar(TILE_CHOOSE_X-8,TILE_CHOOSE_Y+(8*i),{Vertical}
							7,HexNibble[i]);
					PutChar(TILE_CHOOSE_X+(8*i),TILE_CHOOSE_Y-8,{Horizontal}
							7,HexNibble[i]);
				end;
			DrawRectangle(TILE_CHOOSE_X-1,TILE_CHOOSE_Y-1,130,130,1);
		end;
	procedure DrawColorSelectors;
		var i:byte;
		procedure DrawPalSelector(X,Y,W:integer;PalNum:byte);
			var i,j,k:byte;
			begin
				PalNum:=(PalNum*16)+16;
				for i:=0 to 1 do
					for j:=0 to 7 do
						for k:=0 to 5 do
							HLine(X+(i*W),Y+(j*6)+k,W,PalNum+(i*8)+j);
			end;
		begin
			DrawRectangle(COLOR_SEL_X-1,COLOR_SEL_Y-1,18,50,1);
			DrawPalSelector(COLOR_SEL_X,COLOR_SEL_Y,8,CurrDefPal);

			VLine(COLOR_SEL_X-03,COLOR_SEL_Y+((ForeColor and $07)*6),6,15);
			HLine(COLOR_SEL_X+((ForeColor shr 3)*8),COLOR_SEL_Y-03,8,15);

			VLine(COLOR_SEL_X+18,COLOR_SEL_Y+((BackColor and $07)*6),6,14);
			HLine(COLOR_SEL_X+((BackColor shr 3)*8),COLOR_SEL_Y+50,8,14);

			DrawRectangle(PALETTE_SEL_X-1,PALETTE_SEL_Y-1,34,50,1);
			for i:=0 to 3 do
				DrawPalSelector(PALETTE_SEL_X+(i*8),PALETTE_SEL_Y,4,i);
			HLine(PALETTE_SEL_X+(CurrDefPal*8),PALETTE_SEL_Y+50,8,15);
		end;
	procedure DrawStatusBar;
		var S:string;
		begin
			Str(CurrTileSet,S);
			OutText(ST_BAR_X,ST_BAR_Y,7,'PG:'+S);
			Str(((CurrTileSet*256)+CurrTileNum):4,S);
			OutText(ST_BAR_X+40,ST_BAR_Y,7,'TILE:'+S);
			Str(CurrDefPal,S);
			OutText(ST_BAR_X+120,ST_BAR_Y,7,'PAL:'+S);

			OutText(ST_BAR_X,ST_BAR_Y+8,7,'HFLIP:'+YesNo[HFlip]);
			OutText(ST_BAR_X+80,ST_BAR_Y+8,7,'VFLIP:'+YesNo[VFlip]);

			Str(Priority,S);
			OutText(ST_BAR_X,ST_BAR_Y+16,7,'PRI:'+S);

			Str(MapLeft:2,S);
			OutText(ST_BAR_X,ST_BAR_Y+24,7,'L:'+S);
			Str(MapTop:2,S);
			OutText(ST_BAR_X+40,ST_BAR_Y+24,7,'T:'+S);
			Str(MapX:2,S);
			OutText(ST_BAR_X+80,ST_BAR_Y+24,7,'X:'+S);
			Str(MapY:2,S);
			OutText(ST_BAR_X+120,ST_BAR_Y+24,7,'Y:'+S);
		end;
	procedure DrawScreen(ShowIt:boolean);
		begin
			DrawColorSelectors;

			DrawRectangle(TILE_EDIT_X-1,TILE_EDIT_Y-1,50,50,1);
			DisplayRawImageClip(TILE_EDIT_X,TILE_EDIT_Y,TilEdBuf^);

			DrawTileChooseBorder;
			DisplayRawImageClip(TILE_CHOOSE_X,TILE_CHOOSE_Y,TilChosBuf^);
			if BlinkCtrl and $08 <> 0 then
				begin
					DrawRectangle(TILE_CHOOSE_X+((CurrTileNum and $0F)*8)-1,
								  TILE_CHOOSE_Y+((CurrTileNum shr 4  )*8)-1,
								  10,10,7);
				end;

			DrawRectangle(MAP_EDIT_X-1,MAP_EDIT_Y-1,162,146,1);
			DisplayRawImageClip(MAP_EDIT_X,MAP_EDIT_Y,TilMapBuf^);

			DrawStatusBar;

			if ShowIt then
				begin
					DrawMouseCursor;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					X_Put_Pal_RAW(Pal,256,0,false);
					ClearBuffer(ScrBuf^);
				end;

			Inc(BlinkCtrl);
		end;
	function IntToStr(N:longint):string;
		var S:string;
		begin
			Str(N,S);
			IntToStr:=S;
		end;
	function FileExists(FileName:string):boolean;
		var F:file;
		begin
			{$I-}
			FileExists:=false;
			if FileName <> '' then
				begin
					Assign(F,FileName);
					Reset(F,1);
					if IOResult = 0 then
						begin
							FileExists:=true;
							Close(F);
						end;
				end;
			{$I+}
		end;
	function AskConfirmation:boolean;
		var Res:byte;
		begin
			Res:=GtYesNo;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtYesNo;
				end;
			AskConfirmation:=(Res = 1);
		end;
	function ChooseFileForLoading(Prompt:string;var FileName:string):boolean;
		var S:string;
			Ok:boolean;
			Res:byte;
		begin
			S:=FileName;
			Ok:=false;

			GtCls;
			GtWriteln('');
			GtWriteln(Prompt);
			GtWrite(' ');

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;

			if (Res = 1) and (S <> '') then {Se n∆o cancelou, ent∆o}
				begin
					GtWriteln(S);
					if not FileExists(S) then
						begin
							GtWriteln('');
							GtWriteln(' File not found.');
							while not KeyPressed do
								begin
									DrawScreen(false);
									GtDraw;
									WaitVSyncStart;
									Buffer2Screen(ScrBuf^);
									ClearBuffer(ScrBuf^);
								end;
							if ReadKey=#0 then ReadKey;
						end
					else
						begin
							Ok:=true;
						end;
				end;

			if Ok then
				FileName:=S;
			ChooseFileForLoading:=Ok;
		end;
	function ChooseFileForSaving(Prompt:string;var FileName:string):boolean;
		var S:string;
			Ok:boolean;
			Res:byte;
		begin
			S:=FileName;
			Ok:=false;

			GtCls;
			GtWriteln('');
			GtWriteln(Prompt);
			GtWrite(' ');

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;

			if (Res = 1) and (S <> '') then {Se n∆o cancelou, ent∆o}
				begin
					GtWriteln(S);
					if FileExists(S) then
						begin
							GtWriteln('');
							GtWriteln(' File exists.');
							GtWriteln('   Overwrite? (Y/N)');
							Ok:=AskConfirmation;
						end
					else
						begin
							Ok:=true;
						end;
				end;

			if Ok then
				FileName:=S;
			ChooseFileForSaving:=Ok;
		end;
	procedure LoadTileSetDlg;
		var Res:byte;
			S:string;
			StartTile,EndTile,Count:word;
			Junk:integer;
			F:file;
		begin
			GtWriteln('');
			GtWrite(' First Tile: ');
			S:=IntToStr(CurrTileSet*256);

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;
			Val(S,StartTile,Junk);
			if StartTile > Pred(7*256) then
				StartTile:=Pred(7*256);
			GtWriteln(IntToStr(StartTile));

			GtWrite(' # of Tiles: ');
			Assign(F,TileSetFile);
			Reset(F,1);
			S:=IntToStr(FileSize(F) div SizeOf(GenTile));
			Close(F);

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;
			Val(S,Count,Junk);
			EndTile:=StartTile+Count-1;
			if EndTile > Pred(7*256) then
				EndTile:=Pred(7*256);
			Count:=(EndTile-StartTile)+1;
			GtWriteln(IntToStr(Count));

			GtCls;
			GtWriteln('');
			GtWriteln(' Load from tile '+IntToStr(StartTile));
			GtWriteln('        to tile '+IntToStr(EndTile));
			GtWriteln('        ('+IntToStr(Count)+' tiles)');
			GtWriteln(' From file');
			GtWriteln('  '+TileSetFile);

			GtWriteln('');
			GtWriteln(' Is this OK? (Y/N)');

			if AskConfirmation then
				ReadTileSet(StartTile,Count,TileSetFile);
		end;
	procedure SaveTileSetDlg;
		var Res:byte;
			S:string;
			StartTile,EndTile,Count:word;
			Junk:integer;
		begin
			GtWriteln('');
			GtWrite(' From Tile: ');
			S:=IntToStr(CurrTileSet*256);

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;
			Val(S,StartTile,Junk);
			if StartTile > Pred(7*256) then
				StartTile:=Pred(7*256);
			GtWriteln(IntToStr(StartTile));

			GtWrite('        To: ');
			S:=IntToStr(CurrTileSet*256+255);

			Res:=0;
			while Res = 0 do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
					Res:=GtEditStr(S);
				end;
			Val(S,EndTile,Junk);
			if EndTile > Pred(7*256) then
				EndTile:=Pred(7*256);
			GtWriteln(IntToStr(EndTile));

			if StartTile > EndTile then
				begin
					Count:=StartTile;
					StartTile:=EndTile;
					EndTile:=Count;
				end;

			Count := (EndTile - StartTile)+1;

			GtCls;
			GtWriteln('');
			GtWriteln(' Save from tile '+IntToStr(StartTile));
			GtWriteln('        to tile '+IntToStr(EndTile));
			GtWriteln('        ('+IntToStr(Count)+' tiles)');
			GtWriteln(' Into file');
			GtWriteln('  '+TileSetFile);

			GtWriteln('');
			GtWriteln(' Is this OK? (Y/N)');

			if AskConfirmation then
				WriteTileSet(StartTile,Count,TileSetFile);
		end;
	procedure LoadMapFileDlg;
		var Res:byte;
			S:string;
			StartX,StartY,SizeX,SizeY:byte;
			Junk:integer;
		procedure ReadS;
			begin
				Res:=0;
				while Res = 0 do
					begin
						DrawScreen(false);
						GtDraw;
						WaitVSyncStart;
						Buffer2Screen(ScrBuf^);
						ClearBuffer(ScrBuf^);
						Res:=GtEditStr(S);
					end;
			end;
		begin
			GtWriteln('');
			GtWrite(' Insert at X = ');
			S:=IntToStr(MapLeft);

			ReadS;
			Val(S,StartX,Junk);
			if StartX > 127 then
				StartX:=127;
			GtWriteln(IntToStr(StartX));

			GtWrite('           Y = ');
			S:=IntToStr(MapTop);

			ReadS;
			Val(S,StartY,Junk);
			if StartY > 127 then
				StartY:=127;
			GtWriteln(IntToStr(StartY));

			GtWriteln('');

			GtWrite(' Width : ');
			S:=IntToStr(128-StartX);

			ReadS;
			Val(S,SizeX,Junk);
			if StartX+SizeX > 128 then
				SizeX:=128-StartX;
			GtWriteln(IntToStr(SizeX));

			GtWrite(' Height: ');
			S:=IntToStr(128-StartY);

			ReadS;
			Val(S,SizeY,Junk);
			if StartY+SizeY > 128 then
				SizeY:=128-StartY;
			GtWriteln(IntToStr(SizeY));

			GtWriteln('');
			GtWriteln(' Is this OK? (Y/N)');

			if AskConfirmation then
				ReadMapFile(StartX,StartY,SizeX,SizeY,TileMapFile);
		end;
	procedure SaveMapFileDlg;
		var Res:byte;
			S:string;
			StartX,StartY,SizeX,SizeY:byte;
			Junk:integer;
		procedure ReadS;
			begin
				Res:=0;
				while Res = 0 do
					begin
						DrawScreen(false);
						GtDraw;
						WaitVSyncStart;
						Buffer2Screen(ScrBuf^);
						ClearBuffer(ScrBuf^);
						Res:=GtEditStr(S);
					end;
			end;
		begin
			GtWriteln('');
			GtWrite(' Get from X = ');
			S:=IntToStr(MapLeft);

			ReadS;
			Val(S,StartX,Junk);
			if StartX > 127 then
				StartX:=127;
			GtWriteln(IntToStr(StartX));

			GtWrite('          Y = ');
			S:=IntToStr(MapTop);

			ReadS;
			Val(S,StartY,Junk);
			if StartY > 127 then
				StartY:=127;
			GtWriteln(IntToStr(StartY));

			GtWriteln('');

			GtWrite(' Width : ');
			S:=IntToStr(128-StartX);

			ReadS;
			Val(S,SizeX,Junk);
			if StartX+SizeX > 128 then
				SizeX:=128-StartX;
			GtWriteln(IntToStr(SizeX));

			GtWrite(' Height: ');
			S:=IntToStr(128-StartY);

			ReadS;
			Val(S,SizeY,Junk);
			if StartY+SizeY > 128 then
				SizeY:=128-StartY;
			GtWriteln(IntToStr(SizeY));

			GtWriteln('');
			GtWriteln(' Is this OK? (Y/N)');

			if AskConfirmation then
				WriteMapFile(StartX,StartY,SizeX,SizeY,TileMapFile);
		end;
	procedure ConfirmExitDlg;
		begin
			GtCls;
			GtWriteln('');
			GtWriteln(' Exit:');
			GtWriteln(' Are you sure? (Y/N)');
			Prog_Close:=AskConfirmation;
		end;
	procedure FileMenuDlg;
		var Key:char;
		procedure DrawThisDialog;
			begin
				GtCls;
				GtWriteln('');
				GtWriteln(' 1. Load Tileset');
				GtWriteln(' 2. Save Tileset');
				GtWriteln(' 3. Load Map File');
				GtWriteln(' 4. Save Map File');
				GtWriteln(' 5. Load Pal File');
				GtWriteln(' 6. Save Pal File');
				GtWriteln('');
				GtWriteln(' E. Exit this menu');
			end;
		begin
			DrawThisDialog;
			Key:=#0;
			while not (Key in ['E',#27]) do
				begin
					if KeyPressed then
						begin
							Key:=UpCase(ReadKey);
							case Key of
								'1':begin
										if ChooseFileForLoading(' Load Tileset',
																TileSetFile) then
											begin
												LoadTileSetDlg;
											end;
										DrawThisDialog;
									end;
								'2':begin
										if ChooseFileForSaving(' Save Tileset',
															   TileSetFile) then
											begin
												SaveTileSetDlg;
											end;
										DrawThisDialog;
									end;
								'3':begin
										if ChooseFileForLoading(' Load Map File',
																TileMapFile) then
											begin
												LoadMapFileDlg;
											end;
										DrawThisDialog;
									end;
								'4':begin
										if ChooseFileForSaving(' Save Map File',
															   TileMapFile) then
											begin
												SaveMapFileDlg;
											end;
										DrawThisDialog;
									end;
								'5':begin
										if ChooseFileForLoading(' Load Pal File',
																PaletteFile) then
											begin
												ReadPaletteFile(CurrDefPal,PaletteFile);
											end;
										DrawThisDialog;
									end;
								'6':begin
										if ChooseFileForSaving(' Save Pal File',
															   PaletteFile) then
											begin
												WritePaletteFile(CurrDefPal,PaletteFile);
											end;
										DrawThisDialog;
									end;
								#00:ReadKey;
							end;
						end;
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
				end;
		end;
	procedure About;
		begin
			GtCls;
			GtWriteln('');
			GtWriteln(' GENED v1.0');
			GtWriteln('   by Haroldo');
			GtWriteln('');
			GtWriteln(' Keys:');
			GtWriteln(' 12346789 Move map');
			GtWriteln(' xcv Clr/Cpy/Paste');
			GtWriteln(' XY  FlipX/FlipY');
			GtWriteln(' rR  Rotate');
			GtWriteln(' f   File options');
			while not KeyPressed do
				begin
					DrawScreen(false);
					GtDraw;
					WaitVSyncStart;
					Buffer2Screen(ScrBuf^);
					ClearBuffer(ScrBuf^);
				end;
			if ReadKey=#0 then ReadKey;
		end;
	procedure HandleSpecialKeys(Key:char);
		begin
			case Key of
				#72:begin {Cima}
						ShiftTile(CurrTile^,0,7);
						RefreshAll;
					end;
				#80:begin {Baixo}
						ShiftTile(CurrTile^,0,1);
						RefreshAll;
					end;
				#75:begin {Esquerda}
						ShiftTile(CurrTile^,7,0);
						RefreshAll;
					end;
				#77:begin {Direita}
						ShiftTile(CurrTile^,1,0);
						RefreshAll;
					end;
				#73:begin {Page Up}
					end;
				#81:begin {Page Down}
					end;
				#132:
					begin {Ctrl+PgUp}
					end;
				#118:
					begin {Ctrl+PgDwn}
					end;
				#71:begin {Home}
					end;
				#79:begin {End}
					end;
				#82:begin {Ins}
					end;
				#83:begin {Del}
					end;
				#115:
					begin {Ctrl+Esquerda}
					end;
				#116:
					begin {Ctrl+Direita}
					end;
				#25:begin {Alt-P}
						Priority:=Priority xor $01;
					end;
				#35:begin {Alt-H}
						HFlip:=not HFlip;
					end;
				#47:begin {Alt-V}
						VFlip:=not VFlip;
					end;
			end;
		end;
	procedure HandleKeyPresses;
		var Key:char;
		begin
			if KeyPressed then
				begin
					Key:=ReadKey;
					case Key of
						#27:begin {Esc}
								ConfirmExitDlg;
							end;
						#09:begin {Tab}
								CurrTileSet:=(CurrTileSet+1) mod 7;
								CurrTile:=Addr(TileSet^[CurrTileSet,CurrTileNum]);
								RefreshTilEdBuf;
								RefreshTilChosBuf;
							end;
						'1':begin
								if MapTop < 128-18 then
									Inc(MapTop);
								if MapLeft > 0 then
									Dec(MapLeft);
								RefreshTilMapBuf;
							end;
						'2':begin
								if MapTop < 128-18 then
									Inc(MapTop);
								RefreshTilMapBuf;
							end;
						'3':begin
								if MapTop < 128-18 then
									Inc(MapTop);
								if MapLeft < 128-20 then
									Inc(MapLeft);
								RefreshTilMapBuf;
							end;
						'4':begin
								if MapLeft > 0 then
									Dec(MapLeft);
								RefreshTilMapBuf;
							end;
						'5':begin
							end;
						'6':begin
								if MapLeft < 128-20 then
									Inc(MapLeft);
								RefreshTilMapBuf;
							end;
						'7':begin
								if MapTop > 0 then
									Dec(MapTop);
								if MapLeft > 0 then
									Dec(MapLeft);
								RefreshTilMapBuf;
							end;
						'8':begin
								if MapTop > 0 then
									Dec(MapTop);
								RefreshTilMapBuf;
							end;
						'9':begin
								if MapTop > 0 then
									Dec(MapTop);
								if MapLeft < 128-20 then
									Inc(MapLeft);
								RefreshTilMapBuf;
							end;
						'F','f':
							begin
								FileMenuDlg;
							end;
						'x':begin
								FillChar(CurrTile^,SizeOf(CurrTile^),0);
								RefreshAll;
							end;
						'c':begin
								Move(CurrTile^,ClipBoard,SizeOf(CurrTile^));
								RefreshAll;
							end;
						'v':begin
								Move(ClipBoard,CurrTile^,SizeOf(CurrTile^));
								RefreshAll;
							end;
						'X':begin
								HFlipTile(CurrTile^);
								RefreshAll;
							end;
						'Y':begin
								VFlipTile(CurrTile^);
								RefreshAll;
							end;
						'r':begin
								RotateTile(CurrTile^);
								RefreshAll;
							end;
						'R':begin
								RotateTile(CurrTile^);
								RotateTile(CurrTile^);
								RotateTile(CurrTile^);
								RefreshAll;
							end;
						'A','a':
							begin
								About;
							end;
						'S','s':
							begin
								Superimpose:=not Superimpose;
								RefreshTilMapBuf;
							end;
						#00:begin
								Key:=ReadKey;
								HandleSpecialKeys(Key);
							end;
					end;
				end;
		end;
	function IsInside(X,Y,Xr,Yr,Wr,Hr:integer):boolean;
		begin
			IsInside:=true;
			Dec(X,Xr);
			Dec(Y,Yr);
			if (X < 0) or (Y < 0) or (X >= Wr) or (Y >= Hr) then
				IsInside:=false
		end;
	function EncodeAttrs:byte;
		var Attr:byte;
		begin
			Attr:=CurrDefPal shl TIL_PAL_SHFT;
			Attr:=Attr or CurrTileSet;
			if HFlip then
				Attr:=Attr or TIL_HFL_MASK;
			if VFlip then
				Attr:=Attr or TIL_VFL_MASK;
			if Priority = 1 then
				Attr:=Attr or TIL_PRI_MASK;
			EncodeAttrs:=Attr;
		end;
	procedure DecodeAttrs(Attr:byte);
		begin
			CurrDefPal:=(Attr and TIL_PAL_MASK) shr TIL_PAL_SHFT;
			CurrTileSet:=Attr and TIL_SET_MASK;
			HFlip:=(Attr and TIL_HFL_MASK <> 0);
			VFlip:=(Attr and TIL_VFL_MASK <> 0);
			Priority:=0;
			if Attr and TIL_PRI_MASK <> 0 then
				Priority:=1;
		end;
	procedure EditPaletteEntry(PalNum,Color:byte);
		var S:string;
			Step:byte;
			Res:byte;
			R,G,B:integer;
			OldR,OldG,OldB:integer;
			Junk:integer;
		begin
			Step:=0;
			Res:=0;
			R:=Pal[(PalNum*16)+Color+16,0] shr 3;
			G:=Pal[(PalNum*16)+Color+16,1] shr 3;
			B:=Pal[(PalNum*16)+Color+16,2] shr 3;
			OldR:=R; OldG:=G; OldB:=B;
			while (Step <> 4) and (Res <> 2) do
				begin
					GtCls;
					GtWriteln('');
					GtWriteln(' Palette '+IntToStr(PalNum)+', Color '+IntToStr(Color));
					GtWriteln(' R:'+IntToStr(OldR)+' '+' G:'+IntToStr(OldG)+' '+' B:'+IntToStr(OldB));
					case Step of
						0:begin
							GtWrite(' Red:   ');
						  end;
						1:begin
							Val(S,R,Junk);
							R:=R and $07;
							GtWriteln(' Red:   '+IntToStr(R));
							GtWrite(' Green: ');
						  end;
						2:begin
							Val(S,G,Junk);
							G:=G and $07;
							GtWriteln(' Red:   '+IntToStr(R));
							GtWriteln(' Green: '+IntToStr(G));
							GtWrite(' Blue:  ');
						  end;
						3:begin
							Val(S,B,Junk);
							B:=B and $07;
							GtWriteln(' Red:   '+IntToStr(R));
							GtWriteln(' Green: '+IntToStr(G));
							GtWriteln(' Blue:  '+IntToStr(B));
							GtWrite(' Is that OK? (Y,N)');
						  end;
					end;
					Res:=0;
					if Step <> 3 then
						begin
							S:='';
							case Step of
								0:S:=IntToStr(R);
								1:S:=IntToStr(G);
								2:S:=IntToStr(B);
							end;
							while Res = 0 do
								begin
									Res:=GtEditStr(S);
									DrawScreen(false);
									GtDraw;
									WaitVSyncStart;
									Buffer2Screen(ScrBuf^);
									ClearBuffer(ScrBuf^);
								end;
						end
					else
						begin
							Res:=GtYesNo;
							while Res = 0 do
								begin
									DrawScreen(false);
									GtDraw;
									WaitVSyncStart;
									Buffer2Screen(ScrBuf^);
									ClearBuffer(ScrBuf^);
									Res:=GtYesNo;
								end;
						end;
					Inc(Step);
				end;
			if Res <> 2 then {Se n∆o cancelou, ent∆o}
				begin
					Pal[(PalNum*16)+Color+16,0]:=R shl 3;
					Pal[(PalNum*16)+Color+16,1]:=G shl 3;
					Pal[(PalNum*16)+Color+16,2]:=B shl 3;
				end;
		end;
	procedure ColorSelCheckMouse(X,Y:integer);
		var Color:byte;
		begin
			if Mouse_B <> 0 then
				begin
					Color:=(Y div 6) + ((X div 8) * 8);
					if Mouse_B and $02 <> 0 then
						BackColor:=Color
					else
						ForeColor:=Color;
				end;
		end;
	procedure PaletteSelCheckMouse(X,Y:integer);
		begin
			if Mouse_B <> 0 then
				begin
					if Mouse_B and $02 <> 0 then {Bot∆o direito}
						EditPaletteEntry(X div 8, (Y div 6)+((X div 4) mod 2)*8);
					CurrDefPal:=X div 8;
					RefreshTilEdBuf;
					RefreshTilChosBuf;
					RefreshTilMapBuf;
				end;
		end;
	procedure TEdCheckMouse(X,Y:integer);
		var Color:byte;
		begin
			if Mouse_B <> 0 then
				begin
					Color:=ForeColor;
					if Mouse_B and $02 <> 0 then {Bot∆o direito}
						Color:=BackColor;
					SetTilePixel(X div 6, Y div 6, Color, CurrTile^);
					RefreshTilEdBuf;
					RefreshTilChosBuf;
					RefreshTilMapBuf;
				end;
		end;
	procedure TChooseCheckMouse(X,Y:integer);
		begin
			if Mouse_B <> 0 then {Muda o caracter selecionado}
				begin
					CurrTileNum:=((Y div 8) shl 4)+(X div 8);
					CurrTile:=Addr(TileSet^[CurrTileSet,CurrTileNum]);
					RefreshTilEdBuf;
				end;
		end;
	procedure MapEdCheckMouse(X,Y:integer);
		begin
			MapX:=(X div 8)+MapLeft;
			MapY:=(Y div 8)+MapTop;
			if Mouse_B and $02 <> 0 then {Bot∆o direito}
				begin
					CurrTileNum:=TileMap[(Y div 8)+MapTop,(X div 8)+MapLeft,1];
					CurrTile:=Addr(TileSet^[CurrTileSet,CurrTileNum]);
					DecodeAttrs(TileMap[(Y div 8)+MapTop,(X div 8)+MapLeft,0]);
					RefreshTilEdBuf;
					RefreshTilChosBuf;
				end
			else if Mouse_B <> 0 then {Qualquer outro bot∆o}
				begin
					TileMap[(Y div 8)+MapTop,(X div 8)+MapLeft,1]:=CurrTileNum;
					TileMap[(Y div 8)+MapTop,(X div 8)+MapLeft,0]:=EncodeAttrs;
					RefreshTilMapBuf;
				end;
		end;
	procedure CheckMouseEvents;
		begin
			{Seleá∆o de cores}
			if IsInside(Mouse_X,Mouse_Y,COLOR_SEL_X,COLOR_SEL_Y,16,48) then
				ColorSelCheckMouse(Mouse_X-COLOR_SEL_X,Mouse_Y-COLOR_SEL_Y);
			{Seleá∆o de palheta}
			if IsInside(Mouse_X,Mouse_Y,PALETTE_SEL_X,PALETTE_SEL_Y,32,48) then
				PaletteSelCheckMouse(Mouse_X-PALETTE_SEL_X,Mouse_Y-PALETTE_SEL_Y);
			{Ediá∆o do caracter}
			if IsInside(Mouse_X,Mouse_Y,TILE_EDIT_X,TILE_EDIT_Y,48,48) then
				TEdCheckMouse(Mouse_X-TILE_EDIT_X,Mouse_Y-TILE_EDIT_Y);
			{Seleá∆o de caracter}
			if IsInside(Mouse_X,Mouse_Y,TILE_CHOOSE_X,TILE_CHOOSE_Y,128,128) then
				TChooseCheckMouse(Mouse_X-TILE_CHOOSE_X,Mouse_Y-TILE_CHOOSE_Y);
			{Ediá∆o do mapa}
			if IsInside(Mouse_X,Mouse_Y,MAP_EDIT_X,MAP_EDIT_Y,160,144) then
				MapEdCheckMouse(Mouse_X-MAP_EDIT_X,Mouse_Y-MAP_EDIT_Y);
		end;
Begin
	InitGraph13h;
{	LoadFont('FONT.BIN'); }
	GtInit(MAP_EDIT_X,MAP_EDIT_Y,20,18,15,0);
	New(TileSet);
	InitPalette;
	InitMouse;
	InitTiles;
	EncodeGenPal(GenPal);
	Prog_Close:=false;
	while not Prog_Close do
		begin
			ReadMouse;
			CheckMouseEvents;
			HandleKeyPresses;
			DrawScreen(true);
		end;
	CloseGraph13h;
{	Writeln(Byte(ReadKey));
	Writeln(Byte(ReadKey));}
End.