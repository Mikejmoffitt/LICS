unit G13hTerm;
{"Terminal" para a unit Graph13h (CHEIO de bugs!)}
{programado por Haroldo de Oliveira Pinheiro}
interface
	procedure GtInit(Top,Left,Width,Height:integer;Color,BkColor:byte);
	procedure GtDraw;
	procedure GtCls;
	procedure GtGotoXY(X,Y:integer);
	procedure GtWrite(S:string);
	procedure GtWriteLn(S:string);
	function  GtEditStr(var S:string):byte;
	function  GtYesNo:byte;
implementation
	uses Crt,Graph13h;
	var GtX,GtY,GtW,GtH:integer;
		GtColor,GtBkColor:byte;
		CursorX,CursorY:byte;
		Buf:array[0..23] of string;
	function PadStr(S:string;Len:integer):string;
		begin
			while Length(S) < Len do
				S:=S+' ';
			PadStr:=S;
		end;
	procedure GtInit(Top,Left,Width,Height:integer;Color,BkColor:byte);
		begin
			GtX:=Top;
			GtY:=Left;
			GtW:=Width;
			GtH:=Height;
			GtColor:=Color;
			GtBkColor:=BkColor;
			GtCls;
		end;
	procedure GtDraw;
		var i,j:byte;
		begin
			for i:=0 to Pred(GtH) do
				begin
					for j:=0 to 7 do
						HLine(GtX,GtY+(i*8)+j,GtW*8,GtBkColor);
					OutText(GtX,GtY+(i*8),GtColor,Copy(Buf[i],1,GtW));
				end;
		end;
	procedure GtCls;
		var i:byte;
		begin
			for i:=0 to Pred(GtH) do
				Buf[i]:='';
			GtGotoXY(1,1);
		end;
	procedure GtGotoXY(X,Y:integer);
		begin
			if X > GtW then
				X:=GtW;
			if Y > GtH then
				Y:=GtH;

			CursorX:=X;
			CursorY:=Y;

			Buf[CursorY-1]:=PadStr(Buf[CursorY-1],CursorX-1);
		end;
	procedure GtWrite(S:string);
		begin
			Delete(Buf[CursorY-1],CursorX-1,Length(Buf[CursorY-1]));
			Buf[CursorY-1]:=PadStr(Buf[CursorY-1],CursorX-1);
			Buf[CursorY-1]:=Buf[CursorY-1]+S;
			Inc(CursorX,Length(S));
		end;
	procedure GtWriteLn(S:string);
		begin
			GtWrite(S);
			GtGotoXY(1,CursorY+1);
		end;
	function  GtEditStr(var S:string):byte;
		var Done:byte;
			Key:char;
			OldX,OldY:integer;
		begin
			OldX:=CursorX;
			OldY:=CursorY;
			Done:=0;
			if KeyPressed then
				begin
					Key:=ReadKey;
					case Key of
						#08:begin {BackSpace}
								Delete(S,Length(S),1);
							end;
						#13:begin {Enter}
								Done:=1;
							end;
						#27:begin {Esc}
								Done:=2;
							end;
						#00:begin {Teclas especiais}
								Key:=ReadKey;
							end;
					else
						S:=S+Key;
					end;
				end;
			GtWrite(S);
			GtGotoXY(OldX,OldY);
			GtEditStr:=Done;
		end;
	function  GtYesNo:byte;
		begin
			GtYesNo:=0;
			if KeyPressed then
				begin
					case ReadKey of
						'Y','y','S','s':
							begin
								GtYesNo:=1;
							end;
						'N','n',#27:
							begin
								GtYesNo:=2;
							end;
						#00:ReadKey;
					end;
				end;
		end;
End.