#include "genesis.h"
#include "rdc.h"

int RDC_Decompress_aux(inbuff, inbufflen, outbuff)
unsigned char **inbuff;
uint inbufflen;
unsigned char **outbuff;
{
	uint ctrl_bits;
	uint ctrl_mask;
	register unsigned char *inbuff_idx;
	register unsigned char *outbuff_idx;
	unsigned char *inbuff_end;
	uint cmd;
    register uint cnt;
	uint ofs, len;
	register unsigned char *outbuff_src;

	ctrl_mask = 0;
	inbuff_idx = *inbuff;
	outbuff_idx = *outbuff;
	inbuff_end = inbuff_idx + inbufflen;

	while(inbuff_idx < inbuff_end){
		/* Pega novos bits de controle, se necess rio */
		ctrl_mask = ctrl_mask >> 1;
		if(!ctrl_mask){
			ctrl_bits = *inbuff_idx++;
			ctrl_bits |= (*inbuff_idx++) << 8;
			ctrl_mask = 0x8000U;
		}

		/* Apenas copia o caracter atual, se o bit de controle for zero */
		if(!(ctrl_bits & ctrl_mask)){
			*outbuff_idx++ = *inbuff_idx++;
			continue;
		}

		/* Desfaz o c¢digo de compressÆo */
		cmd = ((*inbuff_idx) >> 4) & 0x0F;
		cnt = (*inbuff_idx) & 0x0F;
		inbuff_idx++;

		switch(cmd){
			case 0: /* RLE Curto */
				cnt += 3;
				for(; cnt; cnt--){
					*outbuff_idx++ = *inbuff_idx;
				}
				inbuff_idx++;
			break;

			case 1: /* RLE Longo */
				cnt += (*inbuff_idx++) << 4;
				cnt += 19;
				for(; cnt; cnt--){
					*outbuff_idx++ = *inbuff_idx;
				}
				inbuff_idx++;
			break;

			case 2: /* PadrÆo longo */
				ofs = cnt + 3;
				ofs += (*inbuff_idx++) << 4;
				cnt = *inbuff_idx++;
				cnt += 16;
				outbuff_src = outbuff_idx - ofs;
				for(; cnt; cnt--){
					*outbuff_idx++ = *outbuff_src++;
				}
			break;

			default: /* PadrÆo curto */
				ofs = cnt + 3;
				ofs += (*inbuff_idx++) << 4;
				outbuff_src = outbuff_idx - ofs;
				for(cnt = cmd; cnt; cnt--){
					*outbuff_idx++ = *outbuff_src++;
				}
			break;
		}
	}

	/* Retorna os valores */
	cnt = outbuff_idx - (*outbuff);
	*inbuff = inbuff_idx;
	*outbuff = outbuff_idx;
	return cnt;
}

int RDC_Decompress(src, dest)
unsigned char *src;
unsigned char *dest;
{
	int block_len;
	int decomp_len;
	register int i;

	block_len = 1;

	while(block_len){
		block_len = *src++;
		block_len |= (*src++) << 8;

		if(block_len){

			if(block_len < 0){ /* Copia os dados nÆo-comprimidos */
				decomp_len = -block_len;
				for(i = decomp_len; i; i--){
					*dest++ = *src++;
				}
			}else{ /* Realiza a descompressÆo dos dados */
				decomp_len = RDC_Decompress_aux(&src, block_len, &dest);
			}

		}
	}
}
