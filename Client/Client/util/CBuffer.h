#pragma once
#include <wtypes.h>

// ���̰߳�ȫ������ά�����ʻ��������ӻ��⹦��
class CBuffer
{
public:
	CBuffer(LPCTSTR _debug=NULL);	// ���ڼ�¼һ����ǣ���׷���ͷŵ�ʱ��Ĵ���
	~CBuffer();
public:
	// ���䡣��ʹԭ����û�ͷ�Ҳû��
	BYTE* Alloc(DWORD size);

	// ���
	VOID Clear();
	
	// ��ȡ���ڴ�ָ�롣
	BYTE* GetBuffer() const;

	// ��ȡ��С
	DWORD GetSize() const;

	// ��������
	const BYTE* Copy(const void* src,DWORD len);
	const BYTE* Copy(const CBuffer& src);

	// �й��ڴ�ռ�
	VOID Attach(void* buffer,size_t bufsize);
	VOID Detach();

	// д���ļ�
	BOOL WriteToFile(LPCTSTR filePath) const;

	// ��ֵ
	void memset(byte value=0);
	DWORD Write(void* p,DWORD size,DWORD startPos=0);

	operator const BYTE* () const;
private:
	void operator =(const CBuffer &src);
	void _free();
protected:
	BYTE* m_pBuf;
	DWORD m_bufSize;
	BOOL m_attached;
#ifdef _DEBUG
	LPCTSTR m_debug;
#endif
};

#define ConstReferenceBuffer const CBuffer&		// ������
#define BufferIn CBuffer	// �����߸�����䡢�ͷ�
#define BufferOut CBuffer	// �����߸����ͷš������������

// �ܵ���Buffer,�̶�buffer��С
class CStreamBuffer : public CBuffer
{
public:
	CStreamBuffer();
	~CStreamBuffer();
public:
	// д��,����ʵ��д���ֽ���
	DWORD	Append(const void *pData,DWORD dataSize);

	// ��ȡ,����ʵ�ʶ�ȡ�ֽ���
	DWORD Get(void *pOut,DWORD bytesToRead);
	void ResetWritePos();
	void ResetReadPos();
private:
	void operator =(const CStreamBuffer &src);
	volatile DWORD m_writePos;		// д��λ�ã����Ǹ������±�
	DWORD m_readPos;
};

// �ܵ���Buffer,�Զ�����buffer��С
class CAutoStreamBuffer : public CBuffer
{
public:
	CAutoStreamBuffer();
	~CAutoStreamBuffer();
public:
	// �趨һ���ο�buffer��С
	VOID	SetReferenceSize(DWORD size);

	// д��
	VOID	Append(const void *pData,DWORD dataSize);
	// ����һ��buffer��д�룬��δAccomplishAppendǰ�������ǲ���д���
	BYTE*	AllocAppend(DWORD dataSize);
	// δָ����Сʱ��������\0��Ϊ��β
	BOOL	AccomplishAppend(DWORD dataSize=0);

	// ��ȡ
	DWORD Get(void *pOut,DWORD bytesToRead);
	void ResetWritePos();
	void ResetReadPos();
private:
	void _CheckBuffer(DWORD requestSize);
	void _TryReorganize();
private:
	void operator =(const CAutoStreamBuffer &src);
	DWORD	m_referenceSize;
	volatile DWORD m_writePos;		// д��λ�ã����Ǹ������±�
	DWORD m_readPos;
	DWORD m_readTimes;
};
