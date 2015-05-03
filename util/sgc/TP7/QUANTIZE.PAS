unit Quantize;
interface
	uses Graph13h,TGA;
	procedure QuantizeImg(var Img:RGBImage;var Pal:Palette);
implementation
	const
		BaseColor=1;
		MaxColors=15;
	type
		QuantRec=record
					R,G,B:byte;
					Popularity:longint;
				 end;
	var Popularity:array[0..7,0..7,0..7] of longint;
		Popul2:array[0..3,0..3,0..3] of QuantRec;
		Popul3:array[0..1,0..1,0..1] of QuantRec;
	function IsOnPal(R,G,B:byte;Pal:Palette;Step:byte):boolean;
		var i:byte;
		begin
			IsOnPal:=false;
			for i:=Step+1 to Pred(BaseColor+MaxColors) do
				if (R=Pal[i,0]) and (G=Pal[i,1]) and (B=Pal[i,2]) then
					IsOnPal:=True;
		end;
	procedure CalcPopul2(Pal:Palette;Step:byte);
		var tR,tG,tB,Popul:longint;
			i,j,k:byte;
			l,m,n:byte;
		begin
			for i:=0 to 3 do
				for j:=0 to 3 do
					for k:=0 to 3 do
						begin
							tR:=0;
							tG:=0;
							tB:=0;
							Popul:=0;

							for l:=0 to 1 do
								for m:=0 to 1 do
									for n:=0 to 1 do
										begin
											Inc(tR,Popularity[(i*2)+l,(j*2)+m,(k*2)+n]*((i*2)+l));
											Inc(tG,Popularity[(i*2)+l,(j*2)+m,(k*2)+n]*((j*2)+m));
											Inc(tB,Popularity[(i*2)+l,(j*2)+m,(k*2)+n]*((k*2)+n));
											Inc(Popul,Popularity[(i*2)+l,(j*2)+m,(k*2)+n]);
										end;

							if Popul <> 0 then
								begin
									Popul2[i,j,k].R:=tR div Popul;
									Popul2[i,j,k].G:=tG div Popul;
									Popul2[i,j,k].B:=tB div Popul;
								end;
							Popul2[i,j,k].Popularity:=Popul;
							if IsOnPal(Popul2[i,j,k].R,
									   Popul2[i,j,k].G,
									   Popul2[i,j,k].B,
									   Pal,Step) then
								begin
									Popul2[i,j,k].Popularity:=0;
								end;
						end;
		end;
	procedure CalcPopul3(Pal:Palette;Step:byte);
		var tR,tG,tB,Popul:longint;
			i,j,k:byte;
			l,m,n:byte;
		begin
			for i:=0 to 1 do
				for j:=0 to 1 do
					for k:=0 to 1 do
						begin
							tR:=0;
							tG:=0;
							tB:=0;
							Popul:=0;

							for l:=0 to 1 do
								for m:=0 to 1 do
									for n:=0 to 1 do
										begin
											Inc(tR,Popul2[(i*2)+l,(j*2)+m,(k*2)+n].Popularity*Popul2[(i*2)+l,(j*2)+m,(k*2)+n].R);
											Inc(tG,Popul2[(i*2)+l,(j*2)+m,(k*2)+n].Popularity*Popul2[(i*2)+l,(j*2)+m,(k*2)+n].G);
											Inc(tB,Popul2[(i*2)+l,(j*2)+m,(k*2)+n].Popularity*Popul2[(i*2)+l,(j*2)+m,(k*2)+n].B);
											Inc(Popul,Popul2[(i*2)+l,(j*2)+m,(k*2)+n].Popularity);
										end;

							if Popul <> 0 then
								begin
									Popul3[i,j,k].R:=tR div Popul;
									Popul3[i,j,k].G:=tG div Popul;
									Popul3[i,j,k].B:=tB div Popul;
								end;
							Popul3[i,j,k].Popularity:=Popul;
							if IsOnPal(Popul3[i,j,k].R,
									   Popul3[i,j,k].G,
									   Popul3[i,j,k].B,
									   Pal,Step) then
								begin
									Popul3[i,j,k].Popularity:=0;
								end;
						end;
		end;
	procedure QuantizeImg(var Img:RGBImage;var Pal:Palette);
		var i,j,k:word;
			Step:integer;
			SubStep:byte;
		function FindPopul:boolean;
			var nR,nG,nB:byte;
				MaxPopul:longint;
				i,j,k:byte;
			begin
				MaxPopul:=0;
				for i:=0 to 7 do
					for j:=0 to 7 do
						for k:=0 to 7 do
							begin
								if Popularity[i,j,k] > MaxPopul then
									begin
										nR:=i;
										nG:=j;
										nB:=k;
										MaxPopul:=Popularity[i,j,k];
									end;
							end;
				if MaxPopul <> 0 then
					begin
						Pal[Step,0]:=nR;
						Pal[Step,1]:=nG;
						Pal[Step,2]:=nB;
						Dec(Step);
						Popularity[nR,nG,nB]:=0;
						FindPopul:=True;
					end
				else
					begin
						FindPopul:=False;
					end;
			end;
		function FindPopul2:boolean;
			var nR,nG,nB:byte;
				MaxPopul:longint;
				i,j,k:byte;
			begin
				MaxPopul:=0;
				for i:=0 to 3 do
					for j:=0 to 3 do
						for k:=0 to 3 do
							begin
								if Popul2[i,j,k].Popularity > MaxPopul then
									begin
										nR:=i;
										nG:=j;
										nB:=k;
										MaxPopul:=Popul2[i,j,k].Popularity;
									end;
							end;
				if MaxPopul <> 0 then
					begin
						Pal[Step,0]:=Popul2[nR,nG,nB].R;
						Pal[Step,1]:=Popul2[nR,nG,nB].G;
						Pal[Step,2]:=Popul2[nR,nG,nB].B;
						Dec(Step);
						Popul2[nR,nG,nB].Popularity:=0;
						FindPopul2:=True;
					end
				else
					begin
						FindPopul2:=False;
					end;
			end;
		function FindPopul3:boolean;
			var nR,nG,nB:byte;
				MaxPopul:longint;
				i,j,k:byte;
			begin
				MaxPopul:=0;
				for i:=0 to 1 do
					for j:=0 to 1 do
						for k:=0 to 1 do
							begin
								if Popul3[i,j,k].Popularity > MaxPopul then
									begin
										nR:=i;
										nG:=j;
										nB:=k;
										MaxPopul:=Popul3[i,j,k].Popularity;
									end;
							end;
				if MaxPopul <> 0 then
					begin
						Pal[Step,0]:=Popul3[nR,nG,nB].R;
						Pal[Step,1]:=Popul3[nR,nG,nB].G;
						Pal[Step,2]:=Popul3[nR,nG,nB].B;
						Dec(Step);
						Popul3[nR,nG,nB].Popularity:=0;
						FindPopul3:=True;
					end
				else
					begin
						FindPopul3:=False;
					end;
			end;
		begin
			for i:=0 to 7 do
				for j:=0 to 7 do
					for k:=0 to 7 do
						Popularity[i,j,k]:=0;
			for i:=0 to Pred(Img.Height) do
				for j:=0 to Pred(Img.Width) do
					with Img.Data[Longint(i*Img.Width)+j] do
						Inc(Popularity[R shr 5, G shr 5, B shr 5]);

			Step:=Pred(BaseColor+MaxColors);
			SubStep:=0;

			while (Step+1 <> BaseColor) and (SubStep < (MaxColors div 3)) do
				begin
					if not FindPopul then
						Step:=BaseColor;
					Inc(SubStep);
				end;

			CalcPopul2(Pal,Step);
			while (Step+1 <> BaseColor) and (SubStep < (MaxColors div 3)*2) do
				begin
					if not FindPopul2 then
						Step:=BaseColor;
					Inc(SubStep);
				end;

			CalcPopul3(Pal,Step);
			while Step+1 <> BaseColor do
				begin
					if not FindPopul2 then
						Step:=BaseColor;
					Inc(SubStep);
				end;

			for i:=BaseColor to Pred(BaseColor+MaxColors) do
				begin
					Pal[i,0]:=Pal[i,0] shl 3;
					Pal[i,1]:=Pal[i,1] shl 3;
					Pal[i,2]:=Pal[i,2] shl 3;
				end;
		end;
End.