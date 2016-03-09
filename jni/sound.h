#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

  class MyAudio
  {
#   define BUFFER_FRAMES 441
    public:
      MyAudio()
      {
        noiseReminder = 0;
        noiseValue = (rand() % 6000) - 3000;
      }

      void Noise(short *buffer, unsigned length, unsigned freq)
      {
        short *dst = buffer;
        while(length)
        {
          if(noiseReminder<sampleRate) noiseReminder += freq;
          else { noiseReminder -= sampleRate; noiseValue = (rand() % 6000) - 3000;
          *(dst++) = noiseVal;
          length--;
        }
      }

    protected:
      static short soundbuffer[BUFFER_FRAMES];
  };

static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;

// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    assert(bq == bqPlayerBufferQueue);
    assert(NULL == context);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    if (--nextCount > 0 && NULL != nextBuffer && 0 != nextSize) {
        SLresult result;
        // enqueue another buffer
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
}



    nextCount = count;
    if (nextSize > 0) {
        // here we only enqueue one buffer because it is a long clip,
        // but for streaming playback we would typically enqueue at least 2 buffers to start
        SLresult result;
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
        if (SL_RESULT_SUCCESS != result) {
            return JNI_FALSE;
        }
    }




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
