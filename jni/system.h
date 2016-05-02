#ifndef H_SYSTEM_GLUKALO
# define H_SYSTEM_GLUKALO

# include <cstdlib>

  inline float RandFloat()
  {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  }

  inline float Clamp(float x, float a, float b)
  {
    // http://www.gamedev.net/topic/473207-clamping-a-value-to-a-range-in-c-quickly/#entry4105200
    return x < a ? a : (x > b ? b : x);
  }

#endif // #ifndef H_SYSTEM_GLUKALO
