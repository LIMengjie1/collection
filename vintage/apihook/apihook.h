//=====================================================================
// 
// apihook.h - win32 api hook help
//
// NOTE:
// for more information, please see the readme file.
//
//=====================================================================
#ifndef __APIHOOK_H__
#define __APIHOOK_H__

#if defined(WIN32) && (!defined(_WIN32))
	#define _WIN32
#endif

#ifndef _WIN32
	#error the file must be compiled under windows
#endif


#define WIN32_LEAN_AND_MEAN
#include <windows.h>


//---------------------------------------------------------------------
// ApiHookFunction 
//---------------------------------------------------------------------
struct ApiHookFunction
{
	const char *Name;	// �������ƣ����磺"DirectDrawCreateEx"
	void *HookFn;		// Hook������ַ��
	void *OrigFn;		// ԭ������ַ��ApiHookInstallʱ���ɡ�
};



#ifdef __cplusplus
extern "C" {
#endif


//=====================================================================
// ����һ���޸� IAT��ʽHOOK������ Hook�κδ� DLL import�����ĺ�������
// ���� Hookͨ�� GetProcAddress�õ���ַ�ĺ�����
//=====================================================================

// Name - DLL����, ���� "kernel32.dll"
// Hook - hook����. Name��ΪNULL�������
// ������Ϊ�ɹ�������ֵΪ����
int ApiHookInstall(const char *Name, struct ApiHookFunction *Hook);

// Name - DLL����, ���� "kernel32.dll"
// Hook - hook����. Name��ΪNULL�������
// ������Ϊ�ɹ�������ֵΪ����
int ApiHookRemove(const char *Name, const struct ApiHookFunction *Hook);


//=====================================================================
// ���������޸Ĵ���
//=====================================================================

// �÷�: OrigFn = ApiHookDetour(LoadLibrary(hDLL, "n"), hookProc, 64)
// ˵���������ڴ棬���� srcָ���Դ������ len���ֽڵĴ��룬���дsrc��
// ����ǰ5���ֽ�Ϊ "jmp �º���" ָ������ղű��渴�ƴ�����ǿ����ڴ�
void *ApiHookDetour(void *src, const void *dst, int len);


// �÷�: ApiHookRetour(LoadLibrary(hDLL, "n"), OrigFn, 64)
// ע��: ApiHookRetour�Ժ�, ���ܵ��� ApiHookDetour���ص� OrigFn ��ַ��
// ��Ϊ�õ�ַָ��Ĵ����Ѿ����ͷš�
int ApiHookRetour(void *src, void *restore, int len);



//=====================================================================
// ����������дָ��
//=====================================================================
struct ApiHookCode
{
	unsigned char *address;
	unsigned int codelen;
	unsigned char newfun[16];
	unsigned char srcfun[16];
};

void ApiHookCodeInit(struct ApiHookCode *code, void *SrcFn, void *NewFn);

void ApiHookCodeOn(struct ApiHookCode *code);

void ApiHookCodeOff(struct ApiHookCode *code);


//=====================================================================
// �����ģ�Hook GetProcAddressȻ�����ж��Ƿ�����Ҫ HOOK�Ķ�����
//=====================================================================
typedef FARPROC (WINAPI *ApiHookProcGPA)(HMODULE hModule, LPCSTR lpProcName);

// ��ʼ���滻 GetProcAddress
// NewGetProcAddress Ϊ NULLʱ����ԭ������ GetProcAddress
int ApiHookSetGPA(ApiHookProcGPA NewGetProcAddress);

// ����ԭ���� GetProcAddress
FARPROC GetProcAddress_Orignal(HMODULE hModule, LPCSTR lpProcName);


//=====================================================================
// �߼��ӿ�
//=====================================================================

// ���� HookIATһ������
void *ApiHookIAT(const char *DllName, const char *FunName, void *HookFn);

// ȡ�� COM����ķ���
#define APIHOOKCOM(obj, lvl, idx) ((void**)(*((char**)(obj) + (lvl))))[idx]


// HOOK COM�ӿڣ������Ͻӿ�
void *ApiHookCOM(void *comobj, int nFunIndex, void *NewFun);


// ע�� Dll��Զ�̽��� (Unicode)
// ProcessId - Զ�̽���id
// LibFile   - DLL����
// th        - ����Զ�̼���DLL�̵߳� Handle
// wait      - �Ƿ�ȴ�Զ��Dll���سɹ���0��1�ǣ��������ز� DllMain����
int ApiHookInjectLibW(DWORD ProcessId, PCWSTR LibFile, HANDLE *th, int wait);


// ע�� Dll��Զ�̽��� (Ansi)
// ProcessId - Զ�̽���id
// LibFile   - DLL����
// th        - ����Զ�̼���DLL�̵߳� Handle
// wait      - �Ƿ�ȴ�Զ��Dll���سɹ���0��1�ǣ��������ز� DllMain����
int ApiHookInjectLibA(DWORD ProcessId, PCSTR LibFile, HANDLE *th, int wait);



//=====================================================================
// ����HOOK���ۺϷ���Hook D3D8, D3D9, DDRAW, DINPUT
//=====================================================================

// ����HOOK: ������ D3D�ӿں����øú����������ú�������ֵ���ء�
typedef void* (*AdapterDirect3DCreate8_t)(void *obj, UINT sdkver);
typedef void* (*AdapterDirect3DCreate9_t)(void *obj, UINT sdkver);


int ApiHook_D3D8_Set(AdapterDirect3DCreate8_t adapter);


int ApiHook_D3D9_Set(AdapterDirect3DCreate9_t adapter);




//=====================================================================
// ��־���
//=====================================================================
void ApiHookLogSet(const char *fname);

void ApiHookLog(const char *fmt, ...);



#ifdef __cplusplus
}
#endif

#endif



