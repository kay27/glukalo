#ifndef H_CONFIG_GLUKALO
# define H_CONFIG_GLUKALO

# include <SLES/OpenSLES.h>

  const int NUMBER_OF_LEVELS = 20;
  const int NUMBER_OF_LEVELS_X = 5;
  const int NUMBER_OF_LEVELS_Y = 4;

  const float GAP_MAX_OFFSET = 0.7f; // from mid point

  const float FLY_BACK = 0.34;

  const int START_LEVEL = 0;

  const int SCORE_XOR_CODE = 0x5a13;

  const int NEXT_LEVEL_SCORE = 20;

  const int MAX_COLUMNS = 3;

  const float BIRD_RADIUS = 0.1f;

  const float MISSILE_RADIUS = 0.04f;

  const float BONUS_RADIUS = 0.07f;

  const float ICON_SIZE = 0.17f;

  const float EXPLODE_TIMEOUT = 111111;

  const float MISSILE_DELAY = 500000;

  const int MAX_MISSILES = 3;

  const int AUTOPILOT_TIME_US = 137 * 100 * 1000; // 13.7s
  const int RANDOM_BONUS_US = 10 * 1000 * 1000; // 15s
  const int CHANGE_RADIUS_TIME = 20 * 1000 * 1000; // 20s

  const float GRAVITY_TUNE = 27000;

  const float GAP_HALFSIZE = 0.28;
  const float GAP_HALFSIZE_MIN = 0.26;
  const float GAP_HALFSIZE_MAX = 0.381;

  const float COLUMN_HALFWIDTH = 0.14;

  const float FLOOR_HEIGHT = 0.07;

  const float SEGMENT = 2.0f / (float)(MAX_COLUMNS-1);

  const float TAP_IMPULSE = 0.04;

  const float ACCELERATION = 0.00000015;

  const float H_SPEED = 0.0000005;

  const float H_MISSILE_SPEED = H_SPEED * 2.7;
//  const float H_MISSILE_SPEED = H_SPEED;
//  const float H_MISSILE_SPEED = H_SPEED*1.3;


# define ACTIVITY_NAME "com.kay27.Glukalo.MainActivity"

  const int   SAMPLE_RATE = 44100;
//  const int   MISS_SAMPLE_LENGTH = SAMPLE_RATE * 1 / 10; //0.1 s
  const int   MISS_SAMPLE_LENGTH = SAMPLE_RATE * 3 / 10; //0.3 s
  const int   BONUS_SAMPLE_LENGTH = SAMPLE_RATE * 4 / 10; //0.4 s
  const int   GAMEOVER_SAMPLE_LENGTH = SAMPLE_RATE * 1 / 1; //1.0 s
  const int   D_SAMPLE_LENGTH = SAMPLE_RATE * 4 / 100; //0.04 s
  const int   ENDOFAUTOPILOT_SAMPLE_LENGTH = SAMPLE_RATE * 2 / 100; //0.02 s
  const float PI = 3.14159265358;
  const int MISS_BASE_FREQ = 300;
//  const int   BUFFER_FRAMES = 441;
//  const SLEnvironmentalReverbSettings REVERB_SETTINGS = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

  const float CHAR_HEIGHT    = 0.08;
  const float CHAR_HALFHEIGHT = CHAR_HEIGHT / 2;
  const float CHAR_LINE_SIZE = 0.13;

#endif // #ifndef H_CONFIG_GLUKALO
