#ifndef H_CONFIG_GLUKALO
# define H_CONFIG_GLUKALO

# include <SLES/OpenSLES.h>

  const int NUMBER_OF_LEVELS = 20;

  const float GAP_MAX_OFFSET = 0.7f; // from mid point

  const float FLY_BACK = 0.34;

  const int START_LEVEL = 0;

  const int NEXT_LEVEL_SCORE = 20;
//  const int NEXT_LEVEL_SCORE = 4;

  const int MAX_COLUMNS = 3;

  const float BIRD_RADIUS = 0.1f;

  const float GAP_HALFSIZE = 0.28;
  const float GAP_HALFSIZE_MIN = 0.26;
  const float GAP_HALFSIZE_MAX = 0.381;

  const float COLUMN_HALFWIDTH = 0.14;

  const float FLOOR_HEIGHT = 0.07;

  const float SEGMENT = 2.0f / (float)(MAX_COLUMNS-1);

  const float TAP_IMPULSE = 0.04;

  const float ACCELERATION = 0.00000015;

  const float H_SPEED = 0.0000005;

# define ACTIVITY_NAME "com.kay27.Glukalo.MainActivity"



  const short MY_AUDIO_NOISE_VOLUME = 3000;


  const int MY_AUDIO_SAMPLE_RATE = 44100;

  const int MY_AUDIO_BUFFER_FRAMES = 441;

  const SLEnvironmentalReverbSettings REVERB_SETTINGS = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;


  const float CHAR_HEIGHT    = 0.1;
  const float CHAR_HALFHEIGHT = CHAR_HEIGHT / 2;
  const float CHAR_LINE_SIZE = 0.1;

#endif // #ifndef H_CONFIG_GLUKALO
