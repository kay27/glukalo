#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

SLObjectItf engineObj;
const SLInterfaceID pIDs[1] = {SL_IID_ENGINE};
const SLboolean pIDsRequired[1]  = {SL_TRUE};
SLresult result = slCreateEngine(
	&engineObj, /*��������� �� �������������� ������*/
	0, /*���������� ��������� � ������� �������������� �����*/
	NULL, /*������ �������������� �����, NULL, ���� ��� ��� �� �����*/
	1, /*���������� ����������, ������� ������ ����� ������������ ����������� ������*/
	pIDs, /*������ ID �����������*/
pIDsRequired /*������ ������, �����������, ��������� �� ��������������� ���������. ���� ������� SL_TRUE, � ��������� �� ��������������, ����� ���������� ��������, � ����� �������� SL_RESULT_FEATURE_UNSUPPORTED*/
);
/*��������� ���������. ���� ����� slCreateEngine ���������� ��������� � ������ �� ���������*/
if(result != SL_RESULT_SUCCESS){
	LOGE("Error after slCreateEngine");
	return;
}
/*����� ������������. ������ ���������� ������ ���� ������ this*/
result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE); //��������� ������  � ���������� ������
/*� ���������� � ���� �������� �������� ����������, ���� �� ������������ ���*/
if(result != SL_RESULT_SUCCESS){
	LOGE("Error after Realize engineObj");
	return;
}

#endif // #ifndef H_SOUND_GLUKALO
