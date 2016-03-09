//Buffer:
#define DEFAULT_FRAMES 1000
for (i = 0; i < DEFAULT_FRAMES; ++i) {
    defaultSineBuffer[i] = (sin((2 * 3.14 ) * (i%1000)*0.001))*(32767) ; // 1000 samples
}

// Call back buffer
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    nextBuffer = defaultSineBuffer;
    nextSize = sizeof(defaultSineBuffer);

    result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
}

// Create Engine
JNIEXPORT jboolean JNICALL Java_com_droid_Inspector_createEngine
        (JNIEnv *env, jobject jObject) {

    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(SL_RESULT_SUCCESS != result){
        return JNI_FALSE;
    }

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if(SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }


    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    if(SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }


    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }


    // get the environmental reverb interface
    // this could fail if the environmental reverb effect is not available,
    // either because the feature is not present, excessive CPU load, or
    // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
    }

    return JNI_TRUE;
}

// Create buffer player with 44.1kHz
JNIEXPORT jboolean JNICALL Java_com_droid_Inspector_createBufferQueueAudioPlayer
        (JNIEnv *env, jobject jObect, jint jInt) {
    SLresult result;

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};


    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1,
                        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                        SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
    if(SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }
}


// Play it
JNIEXPORT jboolean JNICALL Java_com_droid_Inspector_playWave {
    result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
}

android opensl
shareeditflag
	
edited Nov 2 '15 at 7:21
	
asked Nov 2 '15 at 6:21
user1774290
12
	
add a comment
	
start a bounty
1 Answer
active
oldest
votes
up vote
0
down vote
	

The right way to generate Sine is as follows:

    A sample rate of 44.1 kHz is 44,100 samples/second
    A 44100 kHz wave is 44,100 cycles/second
    The cycle of a sine function is 2*PI

Now lets code:

#define DEFAULT_FRAMES 1000

    for(i = 0; i < DEFAULT_FRAMES ; i++)
{
  // i is the sample index
  // Straight sine function means one cycle every 2*pi samples:
  // defaultSineBuffer[i] = sin(i); 
  // Multiply by 2*pi--now it's one cycle per sample:
  // defaultSineBuffer[i] = sin((2 * pi) * i); 
  // Multiply by 44,100 samples per second--now it's 44,100 cycles per second:
  // defaultSineBuffer[i] = sin(44100* (2 * pi) * i);
  // Divide by 44,100 samples per second--now it's 44,100 cycles per 44,100
  // samples, which is just what we needed:
  //defaultSineBuffer[i] = sin(44100 * (2 * pi) * i / 44100);
    defaultSineBuffer[i] = sin(2 * pi * i) * (32767);
}













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
