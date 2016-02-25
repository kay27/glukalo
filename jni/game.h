#ifndef H_GAME_GLUKALO
# define H_GAME_GLUKALO

# include "config.h"
# include "shader.h"

  class Bird;
  class Gap;

  class Game
  {
    public:
      Game();
      ~Game();
      void Init();
      void Pause();
      void Resume();
      void Tap();
      void Render();

    protected:
      Bird bird;
      vector<Gap> gaps;
  }

  class Bird
  {
    
  }

#endif // #ifndef H_GAME_GLUKALO
