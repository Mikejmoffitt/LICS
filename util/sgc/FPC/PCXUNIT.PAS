unit PCXUnit;
{Unit para manipular arquivos PCX}
{Programado por Haroldo de Oliveira Pinheiro}
interface
    {$PACKRECORDS 1}
	uses Graph13h;
	type
		PCXHeader = record
						Manufacturer:byte;
						Version:byte;
						Encoding:byte;
						BitsPerPixel:byte;
						XMin,YMin,XMax,YMax:word;
						HDPI,VDPI:word;
						ColorMap16:array[0..47] of byte;
						Reserved:byte;
						NPlanes:byte;
						BytesPerLine:word;
						PaletteInfo:word;
						HScreenSize:word;
						VScreenSize:word;
						Filler:array[0..53] of byte;
					end;
	procedure ReadPCX(var Img:RawImage;FileName:string);
	procedure AllocReadPCX(var Img:PRawImage;FileName:string);
	procedure GetPcxPalette(var Pal:Palette; FileName:string);
implementation
	var PCXFile:file;
		PCXH:PCXHeader;
		DestImg:PRawImage;
	procedure PCXInfo;
		begin
			Writeln('Manufacturer: ',PCXH.Manufacturer);
			Writeln('Version:      ',PCXH.Version);
			Writeln('Encoding:     ',PCXH.Encoding);
			Writeln('BitsPerPixel: ',PCXH.BitsPerPixel);
			Writeln('XMin:         ',PCXH.XMin);
			Writeln('YMin:         ',PCXH.YMin);
			Writeln('XMax:         ',PCXH.XMax);
			Writeln('YMax:         ',PCXH.YMax);
			Writeln('HDPI:         ',PCXH.HDPI);
			Writeln('VDPI:         ',PCXH.VDPI);
			Writeln('NPlanes:      ',PCXH.NPlanes);
			Writeln('BytesPerLine: ',PCXH.BytesPerLine);
			Writeln('PaletteInfo:  ',PCXH.PaletteInfo);
			Writeln('HScreenSize:  ',PCXH.HScreenSize);
			Writeln('VScreenSize:  ',PCXH.VScreenSize);
			Readln;
		end;
	procedure DecodePCX;
		type
			Buffer=array[0..0] of byte;
		var XSize,YSize:word;
			TotalBytes:word;
			Buf:^Buffer;
            PCXBuf,PCXPtr:^Byte;
			LineNumber:word;
		procedure DecodeLine;
			var SubTotal:word;
				Data,Count:byte;
				i:byte;
			procedure GetData;
				begin
                    Data:=PCXPtr^;
                    Inc(PCXPtr);
					Count:=1;
					if (Data and $C0)=$C0 then
						begin
							Count:=Data and $3F;
                            Data:=PCXPtr^;
                            Inc(PCXPtr);
						end;
				end;
			begin
				SubTotal:=0;
				while SubTotal < TotalBytes do
					begin
						GetData;
						FillChar(Buf^[SubTotal],Count,Data);
						Inc(SubTotal,Count);
					end;
			end;
		procedure DisplayLine;
			begin
				Move(Buf^,DestImg^.Data[LineNumber*DestImg^.Width],XSize);
			end;
		begin
			if DestImg <> nil then
				begin
					XSize:=PCXH.XMax-PCXH.XMin+1;
					YSize:=PCXH.YMax-PCXH.YMin+1;
					TotalBytes:=PCXH.NPlanes*PCXH.BytesPerLine;
					GetMem(Buf,TotalBytes);
                    //
                    GetMem(PCXBuf,FileSize(PCXFile));
                    BlockRead(PCXFile,PCXBuf^,FileSize(PCXFile)-FilePos(PCXFile));
                    PCXPtr:=PCXBuf;
                    //
					for LineNumber:=0 to Pred(YSize) do
						begin
							DecodeLine;
							DisplayLine;
						end;
                    //
                    FreeMem(PCXBuf,FileSize(PCXFile));
					FreeMem(Buf,TotalBytes);
				end;
		end;
	procedure GetPCXHeader(FileName:string);
		begin
			Assign(PCXFile,FileName);
			Reset(PCXFile,1);
			BlockRead(PCXFile,PCXH,SizeOf(PCXHeader));
		end;
	procedure ReadPCX(var Img:RawImage;FileName:string);
		begin
			GetPCXHeader(FileName);
			Img.Width:=PCXH.XMax-PCXH.XMin+1;
			Img.Height:=PCXH.YMax-PCXH.YMin+1;
			DestImg:=Addr(Img);
			DecodePCX;
			DestImg:=nil;
			Close(PCXFile);
		end;
	procedure AllocReadPCX(var Img:PRawImage;FileName:string);
		begin
			GetPCXHeader(FileName);
			GetMem(Img,(PCXH.XMax-PCXH.XMin+1)*(PCXH.YMax-PCXH.YMin+1)+4);
			Img^.Width:=PCXH.XMax-PCXH.XMin+1;
			Img^.Height:=PCXH.YMax-PCXH.YMin+1;
			DestImg:=Img;
			DecodePCX;
			DestImg:=nil;
			Close(PCXFile);
		end;
	procedure GetPcxPalette(var Pal:Palette; FileName:string);
		var F:file;
			i,j:byte;
		begin
			Assign(F,FileName);
			Reset(F,1);
			Seek(F,FileSize(F)-SizeOf(Pal));
			BlockRead(F,Pal,SizeOf(Pal));
			for i:=0 to 255 do
				for j:=0 to 2 do
					Pal[i,j]:=Pal[i,j] shr 2;
			Close(F);
		end;
End.