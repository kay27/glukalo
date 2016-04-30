#ifndef H_SYSTEM_GLUKALO
# define H_SYSTEM_GLUKALO

# include <cstdlib>

  inline float RandFloat()
  {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  }


#endif // #ifndef H_SYSTEM_GLUKALO
