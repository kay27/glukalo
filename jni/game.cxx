#include "game.h"

Game::Game()
{
  firstRun = 1;

  yMulValue = 1;

  charWidth = CHAR_HEIGHT;

  maxLevel = 0;

  highScore = MyCallback::GetHighScore();
  if(highScore == -1) highScore = SCORE_XOR_CODE;

  Init();

  InitAudio();
}

Game::~Game()
{
  if(audio != nullptr)
  {
    audio->Stop();
    delete audio;
    audio = nullptr;
  }
}

void Game::Init()
{
  pause = 0;

  if(firstRun)
    MyCallback::Toast("Tap to play");
  firstRun = 0;

  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

  birdProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(birdProgram, birdVertexShader);
  MyShader::AttachFragmentShader(birdProgram, birdFragmentShader);
  MyShader::LinkProgram(birdProgram);

  glUseProgram(birdProgram);

  vPosition          = glGetAttribLocation(birdProgram, "vPosition");
  vTextureCoordinate = glGetAttribLocation(birdProgram, "vTextureCoordinate");

  vColor  = glGetUniformLocation(birdProgram, "vColor");
  vOffset = glGetUniformLocation(birdProgram, "vOffset");
  vRadius = glGetUniformLocation(birdProgram, "vRadius");
  vMul    = glGetUniformLocation(birdProgram, "vMul");
  vEye    = glGetUniformLocation(birdProgram, "vEye");

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUniform4f(vColor, 0, 0.6, 1, 1);
  glUniform1f(vRadius, (float)1.0);
  glUseProgram(0);

  Column::Init();
  Missile::Init();
  Bonus::Init();
  Icon::Init();

  floorProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(floorProgram, floorVertexShader);
  MyShader::AttachFragmentShader(floorProgram, floorFragmentShader);
  MyShader::LinkProgram(floorProgram);
  glUseProgram(floorProgram);
  vFloorPosition = glGetAttribLocation(floorProgram, "vPosition");
  vFloorOffset = glGetUniformLocation(floorProgram, "vOM");
  glEnableVertexAttribArray(vFloorPosition);
  glVertexAttribPointer(vFloorPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glUseProgram(0);

  fontProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(fontProgram, fontVertexShader);
  MyShader::AttachFragmentShader(fontProgram, fontFragmentShader);
  MyShader::LinkProgram(fontProgram);
  glUseProgram(fontProgram);
  vFontPosition = glGetAttribLocation(fontProgram, "vCharPos");
  vFontOffset   = glGetAttribLocation(fontProgram, "vCharTC");
  vFontCharCode = glGetUniformLocation(fontProgram, "vCharCode");
  vFontLineSize = glGetUniformLocation(fontProgram, "vLineSize");
  vFontColor    = glGetUniformLocation(fontProgram, "vCharColor");
  vCharMul      = glGetUniformLocation(fontProgram, "vCharMul");
  vCharOffset   = glGetUniformLocation(fontProgram, "vCharOffset");
  glEnableVertexAttribArray(vFontPosition);
  glEnableVertexAttribArray(vFontOffset);
  glVertexAttribPointer(vFontPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vFontOffset, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUniform1f(vFontLineSize, CHAR_LINE_SIZE);
  glUseProgram(0);

  glClearColor(0.0, 0.0, 0.0, 1.0);

  Restart();
}

void Game::Restart()
{
  y              =  0;
  x              =  0;
  speed          =  0;
  speedVect      = -0.0023;
  gameOver       =  0;
  gameStarted    =  0;
  gameLooped     =  0;
  impulse        =  0;
  level          = -1;
  floorOffset    =  0;
  direction      =  1;
  tapFire        =  0;
  bonus          =  0;

  {
    int hs0 = highScore ^ SCORE_XOR_CODE;
    if(hs0 >= 2 * NEXT_LEVEL_SCORE * NUMBER_OF_LEVELS)
      hs0 = 2 * NEXT_LEVEL_SCORE * NUMBER_OF_LEVELS - 1;
    score = (hs0 - (hs0 % (NEXT_LEVEL_SCORE/2)) - 1) ^ SCORE_XOR_CODE;
  }

//score = 795 ^ SCORE_XOR_CODE;
//score = 73 ^ SCORE_XOR_CODE;

  AddScore();
  scoreRestarted =  (MAX_COLUMNS+1) >> 1;

  for(int i=0; i<MAX_COLUMNS; i++)
  {
    float cx = direction*SEGMENT*i + direction*1.5;
    int cs = (score ^ SCORE_XOR_CODE) + i;
    int cl = GetLevel(cs);
    gaps[i].Restart(cx, cs);
    OnNewColumn(&gaps[i], cx, cs, cl);
  }

  glUseProgram(birdProgram);
  glUniform1f(vRadius, (float)1.0);
  glUseProgram(0);

  gettimeofday(&lastTime, NULL);
}

void Game::Pause()
{
  PauseAudio();
  pause = 1;
}

void Game::Resume()
{
  ResumeAudio();
  Untap();
  gettimeofday(&lastTime, NULL);
  pause = 0;
}

void Game::Resize(int w, int h)
{
  glViewport(0, 0, w, h);
  if(h>0)
  {
    yMulValue = float(w)/h;
    charWidth = CHAR_HEIGHT/yMulValue;
    m.Resize(yMulValue);
    Bonus::Resize(yMulValue);
    Icon::Resize(yMulValue);
    glUseProgram(birdProgram);
    glUniform1f(vMul, yMulValue);
    glUseProgram(fontProgram);
    glUniform1f(vCharMul, yMulValue);
    glUseProgram(0);
  }
}

void Game::Tap(float x, float y)
{
  impulse = 1;

  if(!gameOver)
    gameStarted=1;
}

inline void Game::Untap()
{
  impulse = 0;
}

void Game::Render()
{
  if(pause) return;

  float delta = GetTimeInterval(); if(delta>50000) return; // CPU overload

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float deltaX = delta*H_SPEED*direction;

  int antiGravity = ((level + 1) & 3) == 3;

  if(impulse)
  {
    impulse = 0;
    speed = -TAP_IMPULSE;

    if(tapFire) if(!gameOver) if(gameStarted) m.Start(x, y);
  }

  if(gameStarted)
  {
    speed += delta * ACCELERATION;

    FlyAway(deltaX);

    if(!gameOver)
    {
      MoveColumnsCheckPass(deltaX);
      b.Move(delta, antiGravity, direction);
    }
  }

  m.Move(delta, antiGravity, direction);

  MoveBonus();

  if(!gameOver)
  {
    floorOffset = fmod(floorOffset + deltaX, 1/yMulValue);

    if(antiGravity) y += delta/GRAVITY_TUNE*speed;
      else y -= delta/GRAVITY_TUNE*speed;
  }

  if(!(gameStarted||gameOver))
    DemoAnimation(delta);

  if(y > 1+BIRD_RADIUS*0.9)
  {
    y = 1+BIRD_RADIUS*0.9;
    if(antiGravity) speed = -TAP_IMPULSE / 1.5;
    else speed=0;
  }

  if(y < BIRD_RADIUS+FLOOR_HEIGHT-1)
  {
    y = BIRD_RADIUS+FLOOR_HEIGHT-1;
    GameOver();
  }

  CheckMissiles();
  CheckColumns();
  CheckBonus();

  m.Render();

  glUseProgram(birdProgram);
  if(gameOver)
  {
    gameOverTime += delta;
    float deltaGameOver = (1000000.0 - gameOverTime)/1000000.0;
    if(deltaGameOver<=0)
    {
      glClearColor(0.0, 0.0, 0.0, 1.0);
      deltaGameOver = 1.0;
      Restart();
    }
    else glClearColor((1.0-deltaGameOver)/1.2, 0.0, 0.0, 1.0);
    glUniform1f(vRadius,deltaGameOver);
  }
  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform2f(vEye, ((float)direction)/2.0, 0.5 - antiGravity);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  for(int i=0; i<MAX_COLUMNS; i++)
    gaps[i].Render();

  glUseProgram(floorProgram);
  if(antiGravity)
    glUniform4f(vFloorOffset, floorOffset, yMulValue, 1, 0);
  else
    glUniform4f(vFloorOffset, floorOffset, yMulValue, 0, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

  icon.Render();

  PrintScore();

//  Bonus * q = new Bonus();
  if(bonus)
    b.Render();
//  delete q;

  PlayAudio();
}

void Game::PrintScore()
{
  int q = 0;
  if(!gameOver && gameStarted) q = lastTime.tv_sec & 1;

  int s  = score     ^ SCORE_XOR_CODE;
  int hs = highScore ^ SCORE_XOR_CODE;

  if(s>hs)
  {
    PrintNumber(-1.0+charWidth*0.5, 1-CHAR_HALFHEIGHT, q*0.5+0.5, 0.7-q*0.3, 0.7-q*0.3, s);
    return;
  }

  if(hs > 0)
    PrintNumber(1-charWidth*(GetNumberLength(hs)-0.5), 1-CHAR_HALFHEIGHT, 1, 0.2, 0.2, hs);

  if(s>0)
    PrintNumber(-1.0+charWidth*0.5, 1-CHAR_HALFHEIGHT, q*0.3+0.5, q*0.3+0.5, 1.0, s);

}

int Game::GetNumberLength(int number)
{
  if(number==0) return 1;
  int n=0, s;
  if(number<0) n++;
  for(s=number;s>0;s=s/10,n++);
  return n;
}

void Game::PrintNumber(float xcrd, float ycrd, float r, float g, float b, int number)
{
  glUseProgram(fontProgram);
  glUniform4f(vFontColor, r, g, b, 1.0);
  int n = GetNumberLength(number);
  float x = charWidth * (n - 1) + xcrd;
  do
  {
    glUniform1i(vFontCharCode, 48 + (number % 10));
    glUniform4f(vCharOffset, x, ycrd, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    x -= charWidth;
    number /= 10;
  } while(number > 0);
}

void Game::GameOver()
{
  gameOverTime = 0;
  gameOver=1;
  UpdateHighScore();
}

void Game::ChangeLevel()
{
  int s = score ^ SCORE_XOR_CODE;
  level = GetLevel(s);
  if(level > maxLevel) maxLevel = level;

  {
    int s0 = s % (2 * NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE);

    if(s0 < NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE)
      direction = 1;
    else direction = -1;
  }

  if(s >= 4 * NEXT_LEVEL_SCORE)
    if(tapFire == 0)
      tapFire = 1;

  if(s == 2 * NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE)
  {
    gameLooped++;
    MyCallback::Toast("You win!");
  }
  else if((level == NUMBER_OF_LEVELS-1) && (s >= NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE))
  {
    MyCallback::Toast("Now go back to home!");
  }
  else if(!gameLooped) if(s < NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE)
  {
    char msg[32];
    sprintf(msg, "Level %d", level + 1);
    MyCallback::Toast(msg);
  }

  UpdateHighScore();
}

int Game::GetLevel(int newScore)
{
  if(newScore < 0) newScore=0;

  newScore += START_LEVEL * NEXT_LEVEL_SCORE;
  newScore %= 2 * NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE;

  int newLevel = newScore / NEXT_LEVEL_SCORE;

  if(newLevel >= NUMBER_OF_LEVELS)
  {
    if(newLevel < 2 * NUMBER_OF_LEVELS)
      newLevel = 2 * NUMBER_OF_LEVELS - newLevel - 1;
    else
      newLevel = 0;
  }
  return newLevel;
}

void Game::UpdateHighScore()
{
  if((score ^ SCORE_XOR_CODE) <= (highScore ^ SCORE_XOR_CODE))
    return;

  highScore=score;

  MyCallback::SetHighScore(highScore);
}

void Game::AddScore()
{
  if(scoreRestarted>0) scoreRestarted--;

  int s = (score ^ SCORE_XOR_CODE) + 1;
  score = s ^ SCORE_XOR_CODE;

  if((GetLevel(s) != level) || (s % NEXT_LEVEL_SCORE == 0))
    ChangeLevel();
}

void Game::MoveColumnsCheckPass(float deltaX)
{
  for(int i=0; i<MAX_COLUMNS; i++)
  {
    float xNew = gaps[i].Move(deltaX);

    if(gaps[i].Pass(direction)) AddScore();

    if(direction == 1)
    {
      if(xNew < -1 - COLUMN_HALFWIDTH)
      {
        xNew += SEGMENT * MAX_COLUMNS;
        int xScore = (score ^ SCORE_XOR_CODE) + round((xNew - FLY_BACK) / SEGMENT);
        int xLevel = GetLevel(xScore);
        gaps[i].Restart(xNew, xScore, xLevel);
        OnNewColumn(&gaps[i], xNew, xScore, xLevel);
      }
    }
    else //direction == -1
    {
      if(xNew > 1 + COLUMN_HALFWIDTH)
      {
        xNew -= SEGMENT * MAX_COLUMNS;
        int xScore = (score ^ SCORE_XOR_CODE) + round((FLY_BACK - xNew) / SEGMENT);
        int xLevel = GetLevel(xScore);
        gaps[i].Restart(xNew, xScore, xLevel);
        OnNewColumn(&gaps[i], xNew, xScore, xLevel);
      }
    }
  }
}

float Game::GetTimeInterval()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
  lastTime = now;
  return delta;
}

void Game::DemoAnimation(float delta)
{
  y += speedVect * delta / 253427; // greater -> slower initial animation
  if(y>0.008) {y=0.008; speedVect=-0.0095;}
  if(y<-0.006) {y=-0.006; speedVect=0.011;}
}

void Game::CheckMissiles()
{
  if(gameOver && (m.GetPhase() == 1))
  {
    m.Explode();
    return;
  }

  if(m.GetPhase() != 1)
    return;

  float my = m.GetY();

  for(int i=0; i<MAX_COLUMNS; i++)
  {
    int c = gaps[i].Collision(m.GetX(), my, m.GetR(), yMulValue);
    if(c != 0)
    {
      float gy  = gaps[i].GetY(),
            ghs = gaps[i].GetHalfSize();
      if(gaps[i].GetSolid())
        gy = my;
      float y0 = gy - ghs,
            y1 = gy + ghs;
      float tp = y1, // gap top position
            bp = y0; // gap bottom position
      if(c == -1) { bp = my - GAP_HALFSIZE;   if(bp < -1) bp = -1; }
      else        { tp = my + GAP_HALFSIZE;   if(tp >  1) tp =  1; }
      float newY  = (tp + bp) / 2.0,
            newHS = (tp - bp) / 2.0;
      gaps[i].Freeze(newY, newHS);

      m.Explode();
      return;
    }
  }
}

void Game::CheckColumns()
{
  if (gameOver)
    return;

  for(int i=0; i<MAX_COLUMNS; i++)
  {
    if(gaps[i].Collision(x, y, yMulValue))
    {
      GameOver();
      return;
    }
  }
}

void Game::CheckBonus()
{
  if(!bonus) return;
  if(!b.Collision(x, y)) return;
  if(bonus==1)
  {
    tapFire = 1;
    bonus = 0;
    return;
  }
}

void Game::FlyAway(float deltaX)
{
  if(direction == 1)
  {
    if (x <= -FLY_BACK) return;
    x -= deltaX;
    if(x < -FLY_BACK) x = -FLY_BACK;
    return;
  }

  // direction == -1
  if (x >= FLY_BACK) return;
  x -= deltaX;
  if(x > FLY_BACK) x = FLY_BACK;
}

void Game::MoveBonus()
{
  if(!bonus) return;
  if(bonus==1)
  {
    b.Set(bonusColumn->GetY());
  }
}

void Game::OnNewColumn(Column * c, float cx, int cScore, int cLevel)
{
  int tail = cScore % NEXT_LEVEL_SCORE;
  bool loop = cScore >= NEXT_LEVEL_SCORE * NUMBER_OF_LEVELS;

  if ( (!loop) && (level == 3) && (tail == NEXT_LEVEL_SCORE - 2) )
  { // end of level 4 - get missiles
    bonus = 1;
    bonusColumn = c;
    b.Set(cx, c->GetY(), MUSHROOM_MISSILE);
  }

  else if ( (!loop) && (level == 3) && (tail == NEXT_LEVEL_SCORE - 1) )
    c->MakeSolid();

  else if ( (!loop) && (level == 4) && (tail % (NEXT_LEVEL_SCORE/3) == 0) )
    c->MakeSolid();
}
