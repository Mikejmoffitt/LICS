unit Utils;
{Rotinas diversas}
{programado por Haroldo de Oliveira Pinheiro}
interface
{    uses Graph13h;}

	type
		Vector= record
					X,Y,Z:integer
				end;

		PStrItem=^StrItem;
		StrItem=record
					Next:PStrItem;
					Str:string;
				end;
		StrList=record
					First,Last:PStrItem;
				end;

	function GetToken(var S:string):string;
    function RemoveComment(var S:string):string;
    function RemoveTab(var S:string;TabSize:byte):word;
    function RemoveBlanks(S:string):string;
    function RemoveLeadingZeroes(S:string):string;
    function RemoveQuotes(S:string):string;
    function IntToStr(N:longint):string;
    function IntToStrFmt(N:longint;Digits:byte):string;
	procedure GetVect(var V:Vector;var S:String);
	function ReadNumber(S:string):longint;

	function HexNibble(N:byte):string;
	function HexByte(N:byte):string;
	function HexWord(N:word):string;
	function HexLong(N:longint):string;

	function CompareStr(S1,S2:string):byte;
	function ChangeExt(Name,Ext:string):string;
	function GetBaseDir(Name:string):string;
	function AddQuotes(S:string):string;

    function FileExists(FileName:string):boolean;

    procedure InitStrList(var List:StrList);
    procedure KillStrList(var List:StrList);
    function  AddToStrList(var List:StrList;S:string):PStrItem;
    function  SearchStrList(List:StrList;S:string):PStrItem;
implementation
	const
		Alpha=['A'..'Z','a'..'z','_'];
		Number=['0'..'9','-','+'];
		Symbol=['(',',',')','\'];
	function GetToken(var S:string):string;
		var Token:string;
			i:byte;
		begin
			Token:='';
			i:=1;
			while (S <> '') and (i <= Length(S)) and (Token = '') do
				begin
					if S[i] in Alpha then
						begin
							While (i <= Length(S)) and (S[i] in Alpha+Number) do
								begin
									Token:=Token+S[i];
									Inc(i);
								end;
						end
					else if S[i] in Number then
						begin
							Token:=S[i];
							Inc(i);
							While (i <= Length(S)) and (S[i] in Number-['-','+']) do
								begin
									Token:=Token+S[i];
									Inc(i);
								end;
							Dec(i);
						end
					else if S[i] in Symbol then
						begin
							Token:=S[i];
						end
					else if S[i] = '#' then
						begin
							Token:='#';
							Inc(i);
							While (i <= Length(S)) and (S[i] in Alpha+Number) do
								begin
									Token:=Token+S[i];
									Inc(i);
								end;
						end
					else if S[i] = '/' then
						begin
							Token:='/';
							Inc(i);
							if S[i] = '/' then
								begin
									while (i <= Length(S)) do
										begin
											Token:=Token+S[i];
											Inc(i);
										end;
								end;
						end
					else if S[i] = '"' then
						begin
							Token:='"';
							Inc(i);
							While (i <= Length(S)) and (S[i] <> '"') do
								begin
									Token:=Token+S[i];
									Inc(i);
								end;
						end;
					Inc(i);
				end;
			if (i > Length(S)) and (Token='') then
				S:=''
			else
		    	Delete(S,1,Pred(i));
			GetToken:=Token;
		end;
    function RemoveComment(var S:string):string;
        var i:integer;
            Comment:string;
        begin
            Comment:='';
            i:=1;
            while (i < Pred(Length(S))) and (Comment='') do
                begin
                    if (S[i] = '/') and (S[i+1] = '/') then
                        begin
                            Comment:=Copy(S,i+2,Length(S));
                            Delete(S,i,Length(S));
                        end;
                    Inc(i);
                end;
            RemoveComment:=Comment;
        end;
    function RemoveTab(var S:string;TabSize:byte):word;
        var i:byte;
            TabCnt:word;
        begin
            i:=1;
            TabCnt:=0;
            while S[i] in [' ',#09] do
                begin
                    case S[i] of
                        ' ':Inc(TabCnt);
                        #09:Inc(TabCnt,TabSize);
                    end;
                    Inc(i);
                end;
            Delete(S,1,Pred(i));
            RemoveTab:=TabCnt;
        end;
    function RemoveBlanks(S:string):string;
        var i:byte;
        begin
            i:=1;
            while (i < Length(S)) and (S[i] in [#9,' ']) do
                Inc(i);
            Delete(S,1,i-1);

            i:=Length(S);
            while (i <> 0) and (S[i] in [#9,' ']) do
                Dec(i);
            Delete(S,i+1,Length(S));

            RemoveBlanks:=S;
        end;
    function RemoveLeadingZeroes(S:string):string;
        var i,j:byte;
        begin
            S:=RemoveBlanks(S);

            i:=1;
            if S[i] = '+' then
                Delete(S,1,1)
            else if S[i] = '-' then
                Inc(i);

            j:=i;
            while (S[j] = '0') and (j < Length(S)) do
                Inc(j);
            if j > i then
                Delete(S,i,j-1);

            RemoveLeadingZeroes:=S;
        end;
    function RemoveQuotes(S:string):string;
        begin
            if Length(S) <> 0 then
                begin
                    if S[1] = '"' then
                        Delete(S,1,1);
                    if S[Length(S)] = '"' then
                        Delete(S,Length(S),1);
                end;
            RemoveQuotes:=S;
        end;
    function IntToStr(N:longint):string;
        var S:string;
        begin
            Str(N,S);
            IntToStr:=S;
        end;
    function IntToStrFmt(N:longint;Digits:byte):string;
        var S:string;
        begin
            Str(N:Digits,S);
            IntToStrFmt:=S;
        end;
	procedure GetVect(var V:Vector;var S:String);
		var Token:string;
			Step:byte;
			Junk:integer;
		begin
			V.X:=0;
			V.Y:=0;
			V.Z:=0;
			Token:=GetToken(S);
			Step:=0;
			while (Token <> ')') and (Token <> '') do
				begin
					if Token[1] in Number then
						begin
							case Step of
								0:Val(Token,V.X,Junk);
								1:Val(Token,V.Y,Junk);
								2:Val(Token,V.Z,Junk);
							end;
                            if Junk = 0 then
								Inc(Step);
						end;
					Token:=GetToken(S);
				end;
		end;
	function ReadNumber(S:string):longint;
		var N:Longint;
			Junk:integer;
		begin
			Val(S,N,Junk);
			if Junk <> 0 then
				begin
				end;
			ReadNumber:=N;
		end;

	function HexNibble(N:byte):string;
		const
			HexChar:array[$0..$F] of char=('0','1','2','3',
										   '4','5','6','7',
										   '8','9','A','B',
										   'C','D','E','F');
		begin
			HexNibble:=HexChar[N and $0F];
		end;
	function HexByte(N:byte):string;
		begin
			HexByte:=HexNibble(N shr 4)+HexNibble(N and $0F);
		end;
	function HexWord(N:word):string;
		begin
			HexWord:=HexByte(N shr 8)+HexByte(N and $FF);
		end;
	function HexLong(N:longint):string;
		begin
			HexLong:=HexWord(N shr 16)+HexWord(N and $FFFF);
		end;

	function CompareStr(S1,S2:string):byte;
		var i,MaxSize:word;
		begin
			MaxSize:=Length(S1);
			if MaxSize > Length(S2) then
				MaxSize:=Length(S2);
			i:=1;
			while (i <= MaxSize) and (UpCase(S1[i]) = UpCase(S2[i])) do
				Inc(i);
			if (i > Length(S1)) and (i > Length(S2)) then
				i:=0;
			CompareStr:=i;
		end;
	function ChangeExt(Name,Ext:string):string;
		var i:byte;
		begin
			i:=Length(Name);
            while (Name[i] <> '.') and (i <> 0) do
                Dec(i);
            if i <> 0 then
                ChangeExt:=Copy(Name,1,i)+Ext
            else
                ChangeExt:=Name+Ext;
        end;
    function GetBaseDir(Name:string):string;
        var i:byte;
        begin
            i:=Length(Name);
            while (Name[i] <> '\') and (i > 0) do
                Dec(i);
            GetBaseDir:=Copy(Name,1,i);
        end;
    function AddQuotes(S:string):string;
        begin
            AddQuotes:='"'+S+'"';
        end;

    function FileExists(FileName:string):boolean;
        var F:file;
        begin
            {$I-}
            Assign(F,FileName);
            Reset(F);
            if IOResult <> 0 then
                begin
                    FileExists:=false;
                end
            else
                begin
                    Close(F);
                    FileExists:=true;
                end;
            {$I+}
        end;

    procedure InitStrList(var List:StrList);
        begin
            List.First:=nil;
            List.Last:=nil;
        end;
    procedure KillStrList(var List:StrList);
        var Item:PStrItem;
        begin
            Item:=List.First;
            while Item <> nil do
                begin
                    List.First:=Item^.Next;
                    Dispose(Item);
                    Item:=List.First;
                end;
            InitStrList(List);
        end;
    function  AddToStrList(var List:StrList;S:string):PStrItem;
        var Item:PStrItem;
        begin
            New(Item);
            Item^.Next:=nil;
            Item^.Str:=S;
            if List.First = nil then
                List.First:=Item
            else
                List.Last^.Next:=Item;
            List.Last:=Item;
            AddToStrList:=Item;
        end;
    function  SearchStrList(List:StrList;S:string):PStrItem;
        var Item:PStrItem;
        begin
            Item:=List.First;
            while (Item <> nil) and (Item^.Str <> S) do
                Item:=Item^.Next;
            SearchStrList:=Item;
        end;
End.
