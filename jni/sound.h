#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

SLObjectItf engineObj;
const SLInterfaceID pIDs[1] = {SL_IID_ENGINE};
const SLboolean pIDsRequired[1]  = {SL_TRUE};
SLresult result = slCreateEngine(
	&engineObj, /*Указатель на результирующий объект*/
	0, /*Количество элементов в массиве дополнительных опций*/
	NULL, /*Массив дополнительных опций, NULL, если они Вам не нужны*/
	1, /*Количество интерфесов, которые должен будет поддерживать создаваемый объект*/
	pIDs, /*Массив ID интерфейсов*/
pIDsRequired /*Массив флагов, указывающих, необходим ли соответствующий интерфейс. Если указано SL_TRUE, а интерфейс не поддерживается, вызов завершится неудачей, с кодом возврата SL_RESULT_FEATURE_UNSUPPORTED*/
);
/*Проверяем результат. Если вызов slCreateEngine завершился неуспехом – ничего не поделаешь*/
if(result != SL_RESULT_SUCCESS){
	LOGE("Error after slCreateEngine");
	return;
}
/*Вызов псевдометода. Первым аргументом всегда идет аналог this*/
result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE); //Реализуем объект  в синхронном режиме
/*В дальнейшем я буду опускать проверки результата, дабы не загромождать код*/
if(result != SL_RESULT_SUCCESS){
	LOGE("Error after Realize engineObj");
	return;
}

#endif // #ifndef H_SOUND_GLUKALO
