#ifndef H_SOUND
# define H_SOUND

#include "jni2game.h" //tmp

  // for native audio
# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

  // engine interfaces
  static SLObjectItf engineObject = nullptr;
  static SLEngineItf engineEngine;

  // output mix interfaces
  static SLObjectItf outputMixObject = nullptr;
  static SLEnvironmentalReverbItf outputMixEnvironmentalReverb = nullptr;

  // buffer queue player interfaces
  static SLObjectItf bqPlayerObject = nullptr;
  static SLPlayItf bqPlayerPlay;
  static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
  static SLEffectSendItf bqPlayerEffectSend;
  static SLMuteSoloItf bqPlayerMuteSolo;
  static SLVolumeItf bqPlayerVolume;

  // aux effect on the output mix, used by the buffer queue player
  static const SLEnvironmentalReverbSettings reverbSettings =
      SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

  // pointer and size of the next player buffer to enqueue, and number of remaining buffers
  static short *nextBuffer;
  static unsigned nextSize;
  static int nextCount;

  // this callback handler is called every time a buffer finishes playing
  static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
  {
      // for streaming playback, replace this test by logic to find and fill the next buffer
      if (--nextCount > 0 && nullptr != nextBuffer && 0 != nextSize) {
          SLresult result;
          // enqueue another buffer
          result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
          // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
          // which for this code example would indicate a programming error
          (void)result;
      }
  }

  // create the engine and output mix objects
  static void createEngine()
  {
      SLresult result;

      // create engine
      result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
      (void)result;

      // realize the engine
      result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
      (void)result;

      // get the engine interface, which is needed in order to create other objects
      result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
      (void)result;

      // create output mix, with environmental reverb specified as a non-required interface
      const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
      const SLboolean req[1] = {SL_BOOLEAN_FALSE};
      result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
      (void)result;

      // realize the output mix
      result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
      (void)result;

      // get the environmental reverb interface
      // this could fail if the environmental reverb effect is not available,
      // either because the feature is not present, excessive CPU load, or
      // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
      result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
              &outputMixEnvironmentalReverb);
      if (SL_RESULT_SUCCESS == result) {
          result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                  outputMixEnvironmentalReverb, &reverbSettings);
          (void)result;
      }
      // ignore unsuccessful result codes for environmental reverb, as it is optional for this example

  }


  // create buffer queue audio player
  static void createBufferQueueAudioPlayer()
  {
      SLresult result;

      // configure audio source
      SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
      SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1,
          SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
          SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
      SLDataSource audioSrc = {&loc_bufq, &format_pcm};

      // configure audio sink
      SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
      SLDataSink audioSnk = {&loc_outmix, nullptr};

      // create audio player
      const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
              /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
      const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
              /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
      result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk,
              3, ids, req);
      (void)result;

      // realize the player
      result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
      (void)result;

      // get the play interface
      result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
      (void)result;

      // get the buffer queue interface
      result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
              &bqPlayerBufferQueue);
      (void)result;

      // register callback on the buffer queue
      result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, nullptr);
      (void)result;

      // get the effect send interface
      result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND,
              &bqPlayerEffectSend);
      (void)result;

      // get the volume interface
      result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
      (void)result;

      // set the player's state to playing
      result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
      (void)result;
  }


  // expose the mute/solo APIs to Java for one of the 3 players

  static SLMuteSoloItf getMuteSolo()
  {
      return bqPlayerMuteSolo;
  }

  // expose the volume APIs to Java for one of the 3 players

  static SLVolumeItf getVolume()
  {
      return bqPlayerVolume;
  }

  // enable reverb on the buffer queue player
  static bool enableReverb(bool enabled)
  {
      SLresult result;

      // we might not have been able to add environmental reverb to the output mix
      if (nullptr == outputMixEnvironmentalReverb) {
          return false;
      }

      result = (*bqPlayerEffectSend)->EnableEffectSend(bqPlayerEffectSend,
              outputMixEnvironmentalReverb, (SLboolean) enabled, (SLmillibel) 0);
      // and even if environmental reverb was present, it might no longer be available
      if (SL_RESULT_SUCCESS != result) {
          return false;
      }

      return true;
  }


  // select the desired clip and play count, and enqueue the first buffer if idle
  static bool selectClip(short * clip, int count)
  {
      nextBuffer = clip;
      nextSize = count;
      nextCount = 1;
      if (nextSize > 0) {
          // here we only enqueue one buffer because it is a long clip,
          // but for streaming playback we would typically enqueue at least 2 buffers to start
          SLresult result;
          result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
          if (SL_RESULT_SUCCESS != result) {
              return false;
          }
      }

      return true;
  }

  // shut down the native audio system
  static void shutdown()
  {

      // destroy buffer queue audio player object, and invalidate all associated interfaces
      if (bqPlayerObject != nullptr) {
          (*bqPlayerObject)->Destroy(bqPlayerObject);
          bqPlayerObject = nullptr;
          bqPlayerPlay = nullptr;
          bqPlayerBufferQueue = nullptr;
          bqPlayerEffectSend = nullptr;
          bqPlayerMuteSolo = nullptr;
          bqPlayerVolume = nullptr;
      }

      // destroy output mix object, and invalidate all associated interfaces
      if (outputMixObject != nullptr) {
          (*outputMixObject)->Destroy(outputMixObject);
          outputMixObject = nullptr;
          outputMixEnvironmentalReverb = nullptr;
      }

      // destroy engine object, and invalidate all associated interfaces
      if (engineObject != nullptr) {
          (*engineObject)->Destroy(engineObject);
          engineObject = nullptr;
          engineEngine = nullptr;
      }

  }

#endif // #ifndef H_SOUND
