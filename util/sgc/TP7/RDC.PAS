program RDC;
	uses RDCUnit;
	var Infile, OutFile:File;
Begin
	if ParamCount < 3 then
		begin
			Writeln('RDC v1.0 by Haroldo de Oliveira Pinheiro');
			Writeln('Based on the RDCUNIT, from SWAG');
			Writeln('USAGE: RDC <option> <dest> <source>');
			Writeln('<options>');
			Writeln('  c: Compress source into dest');
			Writeln('  u: Uncompress source into dest');
			Halt(1);
		end;

	if ParamStr(1) = 'c' then
		begin
			Write('Compressing ', ParamStr(3), ' into ', ParamStr(2), ' ... ');

			Assign(Infile,ParamStr(3));
			Reset(Infile,1);

			Assign(OutFile,ParamStr(2));
			Rewrite(OutFile,1);

			Comp_FileToFile(Infile, OutFile);

			Close(Infile);
			Close(OutFile);

			Writeln('Done.');
		end
	else if ParamStr(1) = 'u' then
		begin
			Write('Uncompressing ', ParamStr(3), ' into ', ParamStr(2), ' ... ');

			Assign(Infile,ParamStr(3));
			Reset(Infile,1);

			Assign(OutFile,ParamStr(2));
			Rewrite(OutFile,1);

			DeComp_FileToFile(Infile, OutFile);

			Close(Infile);
			Close(OutFile);

			Writeln('Done.');
		end
	else
		begin
			Writeln('Invalid command: ', ParamStr(1));
			Halt(2);
		end;
End.