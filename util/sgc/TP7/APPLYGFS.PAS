Program ApplyGFS;
{Sega Genesis File System: ApplyGFS}
{programado por Haroldo de Oliveira Pinheiro}
	uses Utils;
	var F,F2:File;
		Buf:array[0..2047] of byte;
		StartOfs:longint;

	procedure Pad(Size:longint);
		begin
			Seek(F,FileSize(F));
			FillChar(Buf,SizeOf(Buf),0);
			while Size > 0 do
				begin
					if Size > SizeOf(Buf) then
						BlockWrite(F,Buf,SizeOf(Buf))
					else
						BlockWrite(F,Buf,Size);
					Dec(Size,SizeOf(Buf));
				end;
		end;
	procedure PatchIt;
		var NumRead:word;
		begin
			Seek(F,StartOfs);
			while not Eof(F2) do
				begin
					BlockRead(F2,Buf,SizeOf(Buf),NumRead);
					BlockWrite(F,Buf,NumRead);
				end;
		end;
Begin
	if ParamCount < 3 then
		begin
			Writeln('APPLYGFS v1.0 by Haroldo de Oliveira Pinheiro');
			Writeln('USAGE: APPLYGFS <binfile> <offset> <file.GFS>');
			Halt(1);
		end;

	Writeln('APPLYGFS v1.0 by Haroldo de Oliveira Pinheiro');
	Writeln('Adding GFS to ',ParamStr(1),' at offset ',ReadNumber(ParamStr(2)),':');

	Assign(F,ParamStr(1));
	Reset(F,1);

	Assign(F2,ParamStr(3));
	Reset(F2,1);

	StartOfs:=ReadNumber(ParamStr(2));
	if FileSize(F) < StartOfs then
		begin
			Writeln('Padding ',StartOfs-FileSize(F),' bytes...');
			Pad(StartOfs-FileSize(F));
		end;

	Writeln('Applying GFS file: ',ParamStr(3),' ...');
	PatchIt;

	Close(F2);
	Close(F);

	Writeln('Done.');
End.