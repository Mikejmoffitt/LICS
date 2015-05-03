Program MakeGFS;
{Sega Genesis File System: MakeGFS}
{programado por Haroldo de Oliveira Pinheiro}
	uses Dos,Utils;

	type
		GenLong=array[0..3] of byte;
		PString=^string;
		GenFName=array[0..11] of char;
		GenDirStruc=packed record
						FName:GenFName;
						Filler:word;
						Offs:GenLong;
						Len:GenLong;
					end;

		PDirItem=^DirItem;
		DirItem=record
					Next:PDirItem;
					Kind:byte;
					FName:PString;
					Struc:GenDirStruc;
				end;
		DirIndex=array[0..1] of PDirItem;
		DirList=record
					First,Last:PDirItem;
					Index:^DirIndex;
					ItemCount,IndexCount:longint;
				end;

	var List:DirList;

	procedure AllocString(var P:PString;S:string);
		begin
			GetMem(P,Length(S)+1);
			P^:=S;
		end;
	procedure FreeString(var P:PString);
		begin
			FreeMem(P,Length(P^)+1);
		end;

	procedure FNameToGenFName(var Gen:GenFName;FName:string);
		var Dir:DirStr;
			Name:NameStr;
			Ext:ExtStr;
			i:byte;
		begin
			FillChar(Gen,SizeOf(Gen),' ');
			FSplit(FName,Dir,Name,Ext);
			for i:=0 to Pred(Length(Name)) do
				begin
					Gen[i]:=UpCase(Name[i+1]);
				end;
			for i:=0 to Pred(Length(Ext)) do
				begin
					Gen[i+8]:=UpCase(Ext[i+1]);
				end;
		end;
	procedure LongToGenLong(var Gen:GenLong;Long:Longint);
		begin
			Gen[0]:=(Long shr 24) and $FF;
			Gen[1]:=(Long shr 16) and $FF;
			Gen[2]:=(Long shr  8) and $FF;
			Gen[3]:=(Long shr  0) and $FF;
		end;
	function CompareGenFName(FName1,FName2:GenFName):integer;
		var i:byte;
			Diff:integer;
		begin
			i:=0;
			Diff:=0;
			while (i < 12) and (Diff = 0) do
				begin
					Diff:=Ord(FName1[i])-Ord(FName2[i]);
					Inc(i);
				end;
			CompareGenFName:=Diff;
		end;

	procedure InitDirList(var List:DirList);
		begin
			List.First:=nil;
			List.Index:=nil;
			List.ItemCount:=0;
			List.IndexCount:=0;
		end;
	function AddToDirList(var List:DirList):PDirItem;
		var Item:PDirItem;
		begin
			New(Item);
			FillChar(Item^,SizeOf(Item^),0);
			Item^.Next:=nil;

			if List.First = nil then {Primeiro elemento inserido na lista}
				List.First:=Item
			else {J  h  itens na lista, insere no fim.}
				List.Last^.Next:=Item;
			List.Last:=Item;

			Inc(List.ItemCount);

			AddToDirList:=Item;
		end;
	procedure BuildDirListIndex(var List:DirList);
		procedure BuildIndex;
			var i:longint;
				Item:PDirItem;
			begin
				Item:=List.First;
				for i:=0 to Pred(List.IndexCount) do
					begin
						List.Index^[i]:=Item;
						if Item <> nil then
							Item:=Item^.Next;
					end;
			end;
		function BubbleSort:boolean;
			var Done:boolean;
				i:longint;
				Item:PDirItem;
			begin
				Done:=true;

				for i:=0 to List.ItemCount-2 do
					begin
						if CompareGenFName( List.Index^[i]^.Struc.FName,
											List.Index^[i+1]^.Struc.FName ) > 0 then
							begin
								Item:=List.Index^[i];
								List.Index^[i]:=List.Index^[i+1];
								List.Index^[i+1]:=Item;
								Done:=False;
							end;
					end;

				BubbleSort:=Done;
			end;
		procedure RebuildLinks;
			var i:longint;
			begin
				List.First:=List.Index^[0];
				for i:=0 to List.ItemCount-2 do
					List.Index^[i]^.Next:=List.Index^[i+1];
				List.Index^[Pred(List.ItemCount)]^.Next:=nil;
				List.Last:=List.Index^[Pred(List.ItemCount)];
			end;
		begin
			if List.IndexCount <> 0 then
				FreeMem(List.Index,List.IndexCount*SizeOf(PDirItem));

			GetMem(List.Index,List.ItemCount*SizeOf(PDirItem));
			List.IndexCount:=List.ItemCount;

			BuildIndex;
			while not BubbleSort do;
			RebuildLinks;
		end;

	procedure ReadListFile(var List:DirList;FileName:string);
		var T:text;
			S:string;
		procedure ProcessNormal(S:string);
			var Item:PDirItem;
			begin
				Writeln('- Adding ',S,' to list');

				if not FileExists(S) then
					begin
						Writeln('	Error: File not found');
					end
				else
					begin
						Item:=AddToDirList(List);
						Item^.Kind:=0;
						AllocString(Item^.FName,S);
						FNameToGenFname(Item^.Struc.FName,Item^.FName^);
					end;
			end;
		begin
			Assign(T,FileName);
			Reset(T);

			InitDirList(List);

			while not Eof(T) do
				begin
					Readln(T,S);
					if S <> '' then
						begin
							if S[1] = '/' then
								begin
								end
							else
								begin
									ProcessNormal(S);
								end;
						end;
				end;

			Close(T);
		end;

	procedure PreProcessHeader(List:DirList);
		var Item:PDirItem;
			F:file;
			CurrOfs:longint;
		begin
			CurrOfs:=(List.ItemCount*SizeOf(GenDirStruc))+SizeOf(GenLong);

			Item:=List.First;
			while Item <> nil do
				begin
					LongToGenLong(Item^.Struc.Offs,CurrOfs);

					Assign(F,Item^.FName^);
					Reset(F,1);
					LongToGenLong(Item^.Struc.Len,FileSize(F));

					Writeln('- Offset ',HexLong(CurrOfs),'h: ',Item^.FName^,' (',FileSize(F),' bytes)');

					Inc(CurrOfs,FileSize(F));
					Close(F);

					Item:=Item^.Next;
				end;
		end;

	procedure WriteHeader(var F:File; List:DirList);
		var Item:PDirItem;
			GLong:GenLong;
		begin
			LongToGenLong(GLong,List.ItemCount);
			BlockWrite(F,GLong,SizeOf(GLong));

			Item:=List.First;
			while Item <> nil do
				begin
					BlockWrite(F,Item^.Struc,SizeOf(Item^.Struc));
					Item:=Item^.Next;
				end;
		end;

	procedure WriteData(var F:File; List:DirList);
		var Item:PDirItem;
		procedure WriteRawData(FileName:string);
			var F2:File;
				Buf:array[0..255] of byte;
				NumRead:word;
			begin
				Writeln('Writing raw data: ',FileName);

				Assign(F2,FileName);
				Reset(F2,1);

				while not Eof(F2) do
					begin
						BlockRead(F2,Buf,SizeOf(Buf),NumRead);
						BlockWrite(F,Buf,NumRead);
					end;

				Close(F2);
			end;
		begin
			Item:=List.First;
			while Item <> nil do
				begin
					case Item^.Kind of
						0:WriteRawData(Item^.FName^);
					end;
					Item:=Item^.Next;
				end;
		end;

	procedure OutputGFS(List:DirList;FileName:string);
		var F:File;
		begin
			Assign(F,FileName);
			Rewrite(F,1);

			WriteHeader(F,List);
			WriteData(F,List);

			Close(F);
		end;
Begin
	if ParamCount < 2 then
		begin
			Writeln('MAKEGFS v1.0 by Haroldo de Oliveira Pinheiro');
			Writeln('USAGE: MAKEGFS <listfile> <file.GFS>');
			Halt(1);
		end;

	Writeln('MAKEGFS v1.0 by Haroldo de Oliveira Pinheiro');

	Writeln('Processing ',ParamStr(1),':');
	ReadListFile(List,ParamStr(1));

	Writeln('Sorting files...');
	BuildDirListIndex(List);

	Writeln('Pre-processing header:');
	PreProcessHeader(List);

	Writeln('Creating ',ParamStr(2),'...');
	OutputGFS(List,ParamStr(2));

	Writeln('Done.');
End.