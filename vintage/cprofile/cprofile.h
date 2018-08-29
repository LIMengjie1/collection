//=====================================================================
// 
// cprofile.h -- light weight profiling tool
//
// NOTE:
// for more information, please see the readme file
//
//=====================================================================

#ifndef __CPROFILE_H__
#define __CPROFILE_H__

//---------------------------------------------------------------------
// Ԥ���� INT64����
//---------------------------------------------------------------------
#ifndef __IINT64_DEFINED
#define __IINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 IINT64;
#else
typedef long long IINT64;
#endif
#endif


//---------------------------------------------------------------------
// ��ʱ�ڵ㣺���ƣ���ʼʱ�䣬�ӽڵ����Ϣ
//---------------------------------------------------------------------
typedef struct _CProfileNode
{
	const char *name;					// ����
	IINT64 starttime;					// ��ʼʱ��
	IINT64 totaltime;					// �ܼ�ʱ��
	IINT64 childtime;					// ����ʱ��
	int totalcall;						// �ܼƴ���
	int reference;						// ���ô���
	const char *srcfile;				// �ļ�����
	const char *funname;				// ��������
	int srcline;						// �ļ��к�
	struct _CProfileNode *parent;		// ��ǰ�ڵ�
	struct _CProfileNode *child;		// ���ӽڵ�
	struct _CProfileNode *sibling;		// �ֵܽڵ�
}	CProfileNode;


//---------------------------------------------------------------------
// ��ʱ�ڵ㣺
//---------------------------------------------------------------------
typedef struct _CProfileManager
{
	CProfileNode *root;					// ��ʼ�ڵ�
	CProfileNode *current;				// ��ǰ�ڵ�
	CProfileNode *eparent;				// ��ǰ���ڵ�
	CProfileNode *echild;				// ��ǰ�ӽڵ�
	IINT64 resettime;					// ��λʱ��
	int framecount;						// ͳ�ƴ���
}	CProfileManager;


#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------
// ��׼ C���ýӿ�
//---------------------------------------------------------------------

// ��ʼһ������Ϊ name��ʱ��ͳ��
void cprofile_start(const char *name, const char *srcfile, int lineno);

// ������һ��ʱ��ͳ��
void cprofile_stop(void);

// ��λͳ����
void cprofile_reset(int needclean);

// ͳ�ƣ�ȡ�ø�������
const char *cprofile_view(float *totaltime, int *times, float *parenttime);

// ������ص����ڵ�
void cprofile_root(void);

// �������һ���ڵ�
int cprofile_first(void);

// �������һ���ڵ�
int cprofile_next(void);

// ��������븸�ڵ�
int cprofile_enter_parent(void);

// ����������ӽڵ�
int cprofile_enter_child(void);



//---------------------------------------------------------------------
// �����ʾ
//---------------------------------------------------------------------
typedef struct CProfileResult
{
	const char *name;			// ��������
	int ncalls;					// ���ô���
	float tottime;				// �ܹ�ʱ��(�����ӵ��õ�ʱ��)
	float per1;					// tottime / ncalls
	float pct1;					// tottime / �������
	float cumtime;				// �ܹ�ʱ��(�������ӵ��õ�ʱ��)
	float per2;					// cumtime / ncalls
	float pct2;					// cumtime / �������
	const char *srcfile;		// Դ�ļ�����
	int srcline;				// Դ�ļ��к�
}	CProfileResult;


// ȡ�ý��
// path  - ·��
// count - �������
// sort  - ����ʽ 0������������
//         sort = 1 (����������)     sort = -1 (���Ƶ���)
//         sort = 2 (��ncalls����)   sort = -2 (ncalls����)
//         sort = 3 (��tottime����)  sort = -3 (tottime����)
//         sort = 4 (��per1����)     sort = -4 (per1����)
//         sort = 5 (��pct1����)     sort = -5 (pct1����)
//         sort = 6 (��cumtime����)  sort = -6 (percent����)
//         sort = 7 (��per2����)     sort = -7 (per2����)
//         sort = 8 (��pct2����)     sort = -8 (pct2����)
// recursive - �Ƿ�ݹ�
CProfileResult *cprofile_result_get(const char *path, int *count, 
	int sort, int recursive);

// �ͷŽ��
void cprofile_result_free(CProfileResult *result);


// ȡ���ı���������� free�ͷţ��Ի��з��ָ�ÿ�У�path, sort�ȼ�����˵��
// rec = 0 ���ݹ飬ֻ��ʾ��ǰ�� rec = 1 �ݹ�
char *cprofile_result_text(const char *path, int recursive);

// �������¼����־�ļ�, n (��־�ļ���)
void cprofile_result_log(const char *path, int recursive, const char *log);

// ������������׼���
void cprofile_result_print(const char *path, int recursive);


// ���÷��
// format = 0 name ncall tottime percall cumtime percall info
// format = 1 name ncall tottime percent cumtime percent info
// format = 2 name ncall tottime percall percent cumtime percall percent
// format = 3 name ncall tottime percall percent cumtime percall percent info
// sort = CSORT_NONE / CSORT_TOTTIME / CSORT_CUMTIME
// hiprecision = 0   ��ȷ�� milisec 0.001s
// hiprecision = 1   ��ȷ�� usec    0.000001s
void cprofile_result_style(int format, int sort, int hiprecision);

// ��¼ȫ������
void cprofile_result_logall(const char *logfile);

// ��ӡȫ������
void cprofile_result_printall(void);


#define CSORT_NONE      (0)         // ������(��¼˳��)
#define CSORT_NAME      (1)			// ����������
#define CSORT_NCALLS    (-2)		// �����ô�������
#define CSORT_TOTTIME	(-3)		// ����ʱ�����
#define CSORT_CUMTIME   (-6)		// �����ʱ�����



#ifdef __cplusplus
}
#endif


//---------------------------------------------------------------------
// ���Ժ궨��
//---------------------------------------------------------------------
#if defined(_DEBUG) || defined(DEBUG)
	#define CPROFILE_USED	1
#elif defined(PROFILE) || defined(_PROFILE)
	#define CPROFILE_USED	1
#else
	#define CPROFILE_USED	0
#endif


#define _CPROFILE_START(name, file, line) cprofile_start(name, file, line)
#define _CPROFILE_STOP() cprofile_stop()

#if CPROFILE_USED
	#define CPROFILE_START(name) _CPROFILE_START(name, __FILE__, __LINE__)
	#define CPROFILE_STOP() _CPROFILE_STOP()
#else
	#define CPROFILE_START(name)
	#define CPROFILE_STOP()
#endif


//---------------------------------------------------------------------
// C++ �ӿ�
//---------------------------------------------------------------------

#ifdef __cplusplus

struct CProfileSample
{
	CProfileSample(const char *name, const char *fn = NULL, int line = 0) { 
		cprofile_start(name, fn, line); 
	}
	~CProfileSample() { 
		cprofile_stop(); 
	}
};

#define _CPROFILE(x)	CProfileSample __cprofile_x__(x, __FILE__, __LINE__)

#if CPROFILE_USED
	#define CPROFILE(x)		_CPROFILE(x)
#else
	#define CPROFILE(x)		
#endif

#endif


#endif


