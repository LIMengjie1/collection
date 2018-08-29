#ifndef __ILZW_H__
#define __ILZW_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


//---------------------------------------------------------------------
// GLOBAL
//---------------------------------------------------------------------
#ifndef ILZW_MAXBITS
#define ILZW_MAXBITS	12
#endif

#if ILZW_MAXBITS > 15
#undef ILZW_MAXBITS
#define ILZW_MAXBITS	15
#endif

#define ILZW_TABSIZE	(1L << ILZW_MAXBITS)
#define ILZW_MASK		(ILZW_TABSIZE - 1)


//---------------------------------------------------------------------
// LZW Descriptor
//---------------------------------------------------------------------
struct ILZWDESC
{
	unsigned char *inptr;		// IO - input ptr
	unsigned char *outptr;		// IO - output ptr
	int inbits;					// IO - input cache bits
	int indata;					// IO - input cache data
	int outbits;				// IO - output cache bits
	int outdata;				// IO - output cache data
	size_t inpos;				// IO - input pointer
	size_t inmax;				// IO - input pointer limit
	size_t outpos;				// IO - output pointer
	size_t outmax;				// IO - output pointer limit
	int error;					// IO - error
	int mode;					// lzw - compress or decompress
	int empty_string;			// lzw - empty string pointer 
	int curr_bit_size;			// lzw - current bit size
	int bit_overflow;			// lzw - is table size greater than limit
	int bit_pos;				// lzw - loading bit position
	int data_pos;				// lzw - loading data position
	int data_len;				// lzw - loading data length
	int entire;					// lzw - loading data entire
	int code;					// lzw - code loaded
	int cc;						// lzw - table size 
	int string_length;			// lzw - string length
	int bit_size;				// lzw - bit size
	short hash[ILZW_TABSIZE];	// lzw - string hash entry
	short next[ILZW_TABSIZE];	// lzw - string hash next
	unsigned char string[ILZW_TABSIZE];		// lzw - decode string
	unsigned char newc[ILZW_TABSIZE];		// lzw - new char
	unsigned short table[ILZW_TABSIZE * 2];	// lzw - table base
};

typedef struct ILZWDESC iLzwDesc;


#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------
// LZW ѹ���㷨����ʵ��4K���(41K�ڴ�)
//---------------------------------------------------------------------
#define ILZW_MODE_COMPRESS		0
#define ILZW_MODE_DECOMPRESS	1

#define ILZW_BITS_DEFAULT	8
#define ILZW_BITS_7			7
#define ILZW_BITS_6			6
#define ILZW_BITS_5			5
#define ILZW_BITS_4			4
#define ILZW_BITS_3			3
#define ILZW_BITS_2			2
#define ILZW_BITS_1			1


// LZW ��ʼ�� 
// mode     - ��;��ILZW_MODE_COMPRESS ����ILZW_MODE_DECOMPRESS
// databits - ����λ����Ĭ����8(ILZW_BITS_DEFAULT), 
//            ������ݶ���Ӣ����ĸ���������ó� 7λ���Դ�����
void ilzw_init(iLzwDesc *lzw, int mode, int databits);

// ѹ���Ժ�����ռ�ö����ڴ�
#define ilzw_deflate_maxsize(size) ((((size) + 2) * ILZW_MAXBITS) / 8 + 10)


// LZW ѹ��֡������λ�ֵ䣩
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_deflate(iLzwDesc *lzw, const void *in, long insize, 
	void *out, long outsize);

// LZW ��ѹ֡������λ�ֵ䣩
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_inflate(iLzwDesc *lzw, const void *in, long insize, 
	void *out, long outsize);

// LZW ��λ�ֵ�
void ilzw_reset(iLzwDesc *lzw);


// LZW �Ͳ��ѹ����ÿ�θ�λ�ֵ䣬�̲߳���ȫ��
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// workmem  - �ⲿ�ṩ��СΪ sizeof(workmem)�Ĺ����ڴ�
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_compress_1(const void *in, long insize, void *out, long outsize,
	void *workmem);


// LZW ���׽�ѹ��ÿ�θ�λ�ֵ䣬�̰߳�ȫ��
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// workmem  - �ⲿ�ṩ��СΪ sizeof(workmem)�Ĺ����ڴ�
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_decompress_1(const void *in, long insize, void *out, long outsize,
	void *workmem);


// LZW ����ѹ����ÿ�θ�λ�ֵ䣩
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_compress(const void *in, long insize, void *out, long outsize);


// LZW ���׽�ѹ��ÿ�θ�λ�ֵ䣩
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// ����ѹ����Ĵ�С�����С�����������ڴ治�� 
long ilzw_decompress(const void *in, long insize, void *out, long outsize);



//---------------------------------------------------------------------
// LZO - ���� minilzo
//---------------------------------------------------------------------

// LZO ԭʼѹ��
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С (ָ�룬���᷵��������ݴ�С��
// wrkmem   - ��Ҫ�ṩ�Ĺ����ڴ� ����СΪ ILZO_WRKMEM_SIZE��
// �ɹ�����0�������Ǵ���
int ilzo1x_1_compress(const unsigned char *in, size_t insize, 
    unsigned char *out, size_t *outsize, void *wrkmem);


// LZO ԭʼ��ѹ
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С (ָ�룬���᷵��������ݴ�С��
// �ɹ�����0�������Ǵ���
int ilzo1x_decompress_safe(const unsigned char *in, size_t insize,
    unsigned char *out, size_t *outsize);


// LZO �����ڴ��С
#define ILZO_WRKMEM_SIZE	((1 << 14) * sizeof(char*))


// LZO ����ѹ�����Զ�������ʱ�����ڴ棩
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С
// �ɹ�����0�������Ǵ���
long ilzo_compress(const void *in, long insize, void *out, long outsize);

// LZO ���׽�ѹ
// in       - ��������ָ��
// insize   - �������ݴ�С
// out      - �������ָ��
// outsize  - ������ݴ�С 
// �ɹ�����0�������Ǵ���
long ilzo_decompress(const void *in, long insize, void *out, long outsize);




//---------------------------------------------------------------------
// �򵥷�װ
//---------------------------------------------------------------------


// ѹ��������� out == NULL������ѹ�����������С�����ڰ��Ż���
// ����ʵ��ѹ�����ȣ���� outsize���������� -1�����ѹ�����󷵻� -2��
typedef long (*iCompressProc)(const void *in, long insize, 
	void *out, long outsize, int level);


// ��ѹ����������ʵ��ѹ�����ȣ���� outsize���������� -1����ѹ���󷵻� -2
typedef long (*iDecompressProc)(const void *in, long insize, 
	void *out, long outsize);


#define IPK_METHOD_LZW		0
#define IPK_METHOD_LZO		1


// LZW / LZO ѹ��
// method = ILZX_METHOD_LZW / ILZX_METHOD_LZO
// in - ��������ָ��
// insize - �������ݴ�С
// out - �������ָ�룬outΪ NULLʱ����ѹ�����������С�����ڰ��Ż���
// outsize - ������ݴ�С
// ����ѹ��������ʵ�ʴ�С����� outsize���������� -1���������� < 0
long ipk_compress(int method, const void *in, long insize, 
	void *out, long outsize, int level);


// LZW / LZO ��ѹ��
// method = ILZX_METHOD_LZW / ILZX_METHOD_LZO
// in - ��������ָ��
// insize - �������ݴ�С
// out - �������ָ�룬��� out == NULL����ֱ�ӷ��ؽ�ѹ�����ݵĴ�С
// outsize - ������ݴ�С
// ���ؽ�ѹ������ʵ�ʴ�С����� outsize���������� -1���������󷵻� < 0
long ipk_decompress(int method, const void *in, long insize, void *out,
	long outsize);


// ��װ�µ�ѹ��/��ѹ��
int ipk_install_method(int method, iCompressProc p1, iDecompressProc p2);


#ifdef __cplusplus
}
#endif

#endif



