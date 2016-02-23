#ifndef H_GAME_GLUKALO
# define H_GAME_GLUKALO

  include "config.h"

  class Game
  {
    public:
      Game();
      ~Game();
      void Pause();
      void Resume();
      void Tap();
      void Render();

    protected:
      
  }

#endif // #ifndef H_GAME_GLUKALO
