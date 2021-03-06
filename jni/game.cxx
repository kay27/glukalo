#include "game.h"

int Game::blockMode = 0;

Game::Game()
{
  blockMode = 0;

  verticalMenu = -1;

  firstRun = 1;

  yMulValue = 1;

  charWidth = CHAR_HEIGHT;

  maxLevel = 0;

  selectedLevelScore = (-1) ^ SCORE_XOR_CODE;

  highScore = MyCallback::GetHighScore();
  muteState = MyCallback::GetMuteState();
  if(highScore == -1) highScore = SCORE_XOR_CODE;
  if(muteState != 1) muteState = 0;
  if(muteState) SoundPlayer::Mute();

  codePass = 0;
  codePass2 = 0;
  codePass3 = 0;

  Init();

  SoundPlayer::Init();
}

Game::~Game()
{
  SoundPlayer::Destroy();
  MyCallback::SetMuteState(muteState);
}

void Game::Init()
{
  pause = 0;
  showMenu = 0;

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
  vState  = glGetUniformLocation(birdProgram, "vState");

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
  ColumnPreview::Init();
//  levelIcon.Init(iconVertexShader, iconLevelFragmentShader, -ICON_SIZE/2-ICON_SIZE/4, -1+ICON_SIZE/2);
//  soundIcon.Init(iconVertexShader, iconSoundFragmentShader,  ICON_SIZE/2+ICON_SIZE/4, -1+ICON_SIZE/2, muteState);
  levelIcon.Init(iconVertexShader, iconLevelFragmentShader, -1+ICON_SIZE/2, 1-ICON_SIZE/2);
  soundIcon.Init(iconVertexShader, iconSoundFragmentShader,  1-ICON_SIZE/2, 1-ICON_SIZE/2, muteState);

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

  blockMode = 2;
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
  bonus          =  NO_BONUS;
  autoPilot      =  0;
  randomTime     =  0;
  changeRadius   =  0;
  addToRadius    =  0;

  {
    int hs0 = highScore ^ SCORE_XOR_CODE;
    if(hs0 >= 2 * NEXT_LEVEL_SCORE * NUMBER_OF_LEVELS)
      hs0 = 2 * NEXT_LEVEL_SCORE * NUMBER_OF_LEVELS - 1;
    score = (hs0 - (hs0 % (NEXT_LEVEL_SCORE/2)) - 1) ^ SCORE_XOR_CODE;

    int sls = selectedLevelScore^SCORE_XOR_CODE;
    if(sls >= 0)
    {
      if (sls >= hs0) selectedLevelScore = (-1) ^ SCORE_XOR_CODE;
      else
        score = (sls - (sls % (NEXT_LEVEL_SCORE/2)) - 1) ^ SCORE_XOR_CODE;
    }
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

  glClearColor(0.0, 0.0, 0.0, 1.0);

  blockMode = 1;
}

void Game::Pause()
{
  SoundPlayer::Pause();
  pause = 1;
}

void Game::Resume()
{
  SoundPlayer::Resume();
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
    levelIcon.Move(-1+ICON_SIZE/2/yMulValue, 1-ICON_SIZE/2);
    soundIcon.Move(1-ICON_SIZE/2/yMulValue, 1-ICON_SIZE/2);
    glUseProgram(birdProgram);
    glUniform1f(vMul, yMulValue);
    glUseProgram(fontProgram);
    glUniform1f(vCharMul, yMulValue);
    glUseProgram(0);
  }
}

void Game::Tap(float x, float y)
{
  if(showMenu) { SelectLevel(x, y); return; }

  if(levelIcon.Tap(x, y))
  {
    showMenu = 1;
    return;
  }
  else if(soundIcon.Tap(x, y))
  {
    muteState = !muteState;
    if(muteState) SoundPlayer::Mute(); else SoundPlayer::Unmute();
    soundIcon.SetState(muteState);
    return;
  }

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
  if(blockMode == 2) { blockMode = 3; Restart(); }
  if(blockMode != 1) return;

  if(pause) return;

  float delta = GetTimeInterval();
  bool cpu_overload = delta > 100000;

  if(showMenu || (!cpu_overload))
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(showMenu) return RenderMenu(delta);

  if(cpu_overload) return;

  Logic(delta);


  for(int i=0; i<MAX_COLUMNS; i++)
    gaps[i].Render();

  glUseProgram(floorProgram);
  if(antiGravity)
    glUniform4f(vFloorOffset, floorOffset, yMulValue, 1, 0);
  else
    glUniform4f(vFloorOffset, floorOffset, yMulValue, 0, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

  levelIcon.Render();
  soundIcon.Render();

  PrintScore();

  if(bonus!=NO_BONUS) b.Render();

  if(changeRadius)
    m.Render( ((float)1.0) + ((float)addToRadius) * ((float)changeRadius) / ((float)CHANGE_RADIUS_TIME) );
  else m.Render(1);

  glUseProgram(birdProgram);
  if(gameOver)
  {
    gameOverTime += delta;
    float deltaGameOver = (1000000.0 - gameOverTime)/1000000.0;
    if(deltaGameOver<=0)
    {
      glClearColor(0.0, 0.0, 0.0, 1.0);
      deltaGameOver = 1.0;
      blockMode = 2;
    }
    else glClearColor((1.0-deltaGameOver)/1.2, 0.0, 0.0, 1.0);
    glUniform1f(vRadius,deltaGameOver*( ((float)1.0) + ((float)addToRadius) * ((float)changeRadius) / ((float)CHANGE_RADIUS_TIME) ));
  }
  else if(changeRadius)
  {
    if(changeRadius > delta) changeRadius -= delta; else changeRadius = 0;
    glUniform1f(vRadius, ((float)1.0) + ((float)addToRadius) * ((float)changeRadius) / ((float)CHANGE_RADIUS_TIME));
    if(!changeRadius) addToRadius = 0;
  }
  glUniform4f(vOffset, x, y, changeSpeed, float(randomTime)/float(RANDOM_BONUS_US));
  glUniform4f(vState, ((float)direction)/2.0, 0.5 - antiGravity, 
    (autoPilot>0) + ((tapFire>0)<<1) + ((randomTime>0)<<2), float(autoPilot)/float(AUTOPILOT_TIME_US));
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Game::PrintScore()
{
  int q = 0;
  if(!gameOver && gameStarted) q = lastTime.tv_sec & 1;

  int s  = score     ^ SCORE_XOR_CODE;
  int hs = highScore ^ SCORE_XOR_CODE;

  if(s>hs)
  {
    PrintNumber(-1.0+charWidth*0.5+ICON_SIZE/yMulValue, 1-CHAR_HALFHEIGHT, q*0.5+0.5, 0.7-q*0.3, 0.7-q*0.3, s);
    return;
  }

  if(hs > 0)
    PrintNumber(1-charWidth*(GetNumberLength(hs)-0.5)-ICON_SIZE/yMulValue, 1-CHAR_HALFHEIGHT, 1, 0.2, 0.2, hs);

  if(s>0)
    PrintNumber(-1.0+charWidth*0.5+ICON_SIZE/yMulValue, 1-CHAR_HALFHEIGHT, q*0.3+0.5, q*0.3+0.5, 1.0, s);

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
  SoundPlayer::PlayGameOver();
  gameOverTime = 0;
  gameOver=1;
  UpdateHighScore();
  int sls = selectedLevelScore^SCORE_XOR_CODE;
  if(sls == -1) return;
  selectedLevelScore = score;
}

void Game::ChangeLevel()
{
  int s = score ^ SCORE_XOR_CODE;
  level = GetLevel(s);
  if(level > maxLevel) maxLevel = level;

  {
    int s0 = s % (2 * NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE);

    int oldDirection = direction;
    if(s0 < NUMBER_OF_LEVELS * NEXT_LEVEL_SCORE)
      direction = 1;
    else direction = -1;
    if(oldDirection != direction)
      OnDirectionChange();
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

    if(gaps[i].Pass(direction))
    {
      AddScore();
      SoundPlayer::PlayMiss((score^SCORE_XOR_CODE)%NEXT_LEVEL_SCORE);
    }

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
    if(gaps[i].Collision(x, y, BIRD_RADIUS * (1+addToRadius), yMulValue))
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
  switch(bonus)
  {
    case MUSHROOM_MISSILE:
      tapFire = 1;
      break;
    case AUTOPILOT:
      autoPilot = AUTOPILOT_TIME_US;
      break;
    case RANDOM_BONUS:
      if(rand()%2)
      {
        randomTime = RANDOM_BONUS_US;
        if(rand()%2 == 0) changeSpeed = 2; else changeSpeed = 0.5;
      }
      else
      {
        changeRadius = CHANGE_RADIUS_TIME;
        addToRadius = ((float)(rand()%2)) - ((float)0.5);
      }
      break;
  }
  SoundPlayer::PlayBonus();
  bonus = NO_BONUS;
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
    bonus = MUSHROOM_MISSILE;
    bonusColumn = c;
    b.Set(cx, c->GetY(), bonus);
  }

  else if ( (!loop) && (level == 3) && (tail == NEXT_LEVEL_SCORE - 1) )
    c->MakeSolid();

  else if ( (!loop) && (level == 4) && (tail % (NEXT_LEVEL_SCORE/3) == 0) )
    c->MakeSolid();

  else if ( (!loop) && (level == 0) && (tail == NEXT_LEVEL_SCORE/5) )
  { bonus = AUTOPILOT; b.Set(cx + SEGMENT/2, RandFloat()/2, bonus); }

  else if ( (loop || (level != 0)) && (tail == NEXT_LEVEL_SCORE/2) )
  { bonus = AUTOPILOT; b.Set(cx + direction * SEGMENT/2, RandFloat()/2, bonus); }

  else if(  ( (!loop) && (tail == (NEXT_LEVEL_SCORE/4) - 1) )
    || ( (loop) && (tail == NEXT_LEVEL_SCORE - (NEXT_LEVEL_SCORE/4)) )  )
  { bonus = RANDOM_BONUS; b.Set(cx + direction * SEGMENT/2, RandFloat()/2, bonus); }
}

void Game::OnDirectionChange()
{
  if(m.GetPhase())
    m.Explode();
}

void Game::RenderMenu(float delta)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  int i;
  if(yMulValue != verticalMenu)
  {
    if(yMulValue <= 1) //portrait
    {
      for (i = 0; i < NUMBER_OF_LEVELS; i++)
      {

        menu_x[i] =  ( ((float)(i % NUMBER_OF_LEVELS_X)) /  NUMBER_OF_LEVELS_X     + 0.5f/ NUMBER_OF_LEVELS_X     - 0.5f ) * 2.0f;
        menu_y[i] = -( ((float)(i / NUMBER_OF_LEVELS_X)) / (NUMBER_OF_LEVELS_Y<<1) + 0.5f/(NUMBER_OF_LEVELS_Y<<1) - 0.5f ) * 2.0f;
      }
      for (i = 0; i < NUMBER_OF_LEVELS; i++)
      {
        menu_x[i+NUMBER_OF_LEVELS] = menu_x[i];
        menu_y[i+NUMBER_OF_LEVELS] = menu_y[i] - 1;
      }
    }
    else //landscape
    {
      for (i = 0; i < NUMBER_OF_LEVELS; i++)
      {
        menu_x[i] =  ( ((float)(i % NUMBER_OF_LEVELS_X)) / (NUMBER_OF_LEVELS_X<<1) + 0.5f/(NUMBER_OF_LEVELS_X<<1) - 0.5f ) * 2.0f;
        menu_y[i] = -( ((float)(i / NUMBER_OF_LEVELS_X)) /  NUMBER_OF_LEVELS_Y     + 0.5f/ NUMBER_OF_LEVELS_Y     - 0.5f ) * 2.0f;
      }
      for (i = 0; i < NUMBER_OF_LEVELS; i++)
      {
        menu_x[i+NUMBER_OF_LEVELS] = menu_x[i] + 1;
        menu_y[i+NUMBER_OF_LEVELS] = menu_y[i];
      }
    }
    verticalMenu = yMulValue;
  }

  int hs=(highScore ^ SCORE_XOR_CODE) / NEXT_LEVEL_SCORE;
  for (i = 0; i < NUMBER_OF_LEVELS; i++)
  {
    if((i<=hs)||(codePass>=8)) cp.Render(menu_x[i], menu_y[i], i, yMulValue <= 1);
      else cp.Render(menu_x[i], menu_y[i], 999, yMulValue <= 1);
    if((i+NUMBER_OF_LEVELS<=hs)||(codePass>=8))
      cp.Render(menu_x[NUMBER_OF_LEVELS+i], menu_y[NUMBER_OF_LEVELS+i], NUMBER_OF_LEVELS-i-1, yMulValue <= 1);
    else
      cp.Render(menu_x[NUMBER_OF_LEVELS+i], menu_y[NUMBER_OF_LEVELS+i], 999, yMulValue <= 1);
  }

  for (i = 0; i < NUMBER_OF_LEVELS; i++)
  {
    float x0=menu_x[i];
    if(i+1>9) x0-=CHAR_HEIGHT/2/yMulValue;
    PrintNumber(x0, menu_y[i], 1, 1, 1, i + 1);
    x0=menu_x[NUMBER_OF_LEVELS + i];
    if(NUMBER_OF_LEVELS - i > 9) x0-=CHAR_HEIGHT/2/yMulValue;
    PrintNumber(x0, menu_y[NUMBER_OF_LEVELS+i], 1, 1, 1, NUMBER_OF_LEVELS - i);
  }

  if(!(yMulValue <= 1))
    for(float q=-0.95;q<0.99;q+=0.1)
      Missile::Render(0, q);

}

void Game::SelectLevel(float x, float y)
{
  bool portrait = yMulValue<=1;
  int ix, iy, newLevel, back;
  if(portrait)
  {
    iy = floor((0.5-y/2)*(NUMBER_OF_LEVELS_Y<<1));
    ix = floor((0.5+x/2)*NUMBER_OF_LEVELS_X);
    if(iy<NUMBER_OF_LEVELS_Y)
    {
      newLevel = iy*NUMBER_OF_LEVELS_X + ix;
      back = 0;
    }
    else
    {
      newLevel = (iy-NUMBER_OF_LEVELS_Y)*NUMBER_OF_LEVELS_X + ix;
      back = 1;
    }
  }
  else
  {
    iy = floor((0.5-y/2)*NUMBER_OF_LEVELS_Y);
    ix = floor((0.5+x/2)*(NUMBER_OF_LEVELS_X<<1));
    if(ix<NUMBER_OF_LEVELS_X)
    {
      newLevel = iy*NUMBER_OF_LEVELS_X + ix;
      back = 0;
    }
    else
    {
      newLevel = iy*NUMBER_OF_LEVELS_X + (ix-NUMBER_OF_LEVELS_X);
      back = 1;
    }
  }

  int s  = score ^ SCORE_XOR_CODE;
  int hs = highScore ^ SCORE_XOR_CODE;
  int newLevelScore = NEXT_LEVEL_SCORE * newLevel + back*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS;

  if(newLevelScore>hs)
  {
    if(codePass<8)
    {
      if((newLevel == 12-5*back) && ((codePass&1)==back)) codePass++;
      else codePass = 0;
      return;
    }
    if(codePass2 < 8)
    {
      if((newLevel == 11-7*back) && ((codePass2&1)==back)) codePass2++;
      else codePass2 = 0;
      if(codePass2>=8) { highScore = (2*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS+2307) ^ SCORE_XOR_CODE; showMenu = 0; }
      return;
    }
    if(codePass3 < 29)
    {
      if((!newLevel) && back) codePass3++;
      else codePass3 = 0;
      if(codePass3>=29) {score = highScore = SCORE_XOR_CODE; codePass=0; codePass2=0; showMenu = 0; }
      return;
    }
  }

  if(s%(2*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS)==newLevelScore%(2*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS))
  {
    showMenu=0;
    return;
  }

  if(hs%(2*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS)==newLevelScore%(2*NEXT_LEVEL_SCORE*NUMBER_OF_LEVELS))
    selectedLevelScore = (-1) ^ SCORE_XOR_CODE;
  else
    selectedLevelScore = newLevelScore ^ SCORE_XOR_CODE;

  blockMode = 2;
  showMenu=0;
}

void Game::Logic(const float delta)
{
  float deltaX = delta * H_SPEED * direction;
  if(randomTime)
  {
    deltaX *= changeSpeed;
    if(randomTime > delta) randomTime -= delta;
      else randomTime=0;
    if(randomTime == 0)
      SoundPlayer::PlayEndOfAutoPilot();
  }

  antiGravity = ((level + 1) & 3) == 3;

  if(gameStarted)
  {
    speed += delta * ACCELERATION;

    if(!gameOver)
    {
      if(impulse)
      {
        impulse = 0;
        speed = -TAP_IMPULSE;
        if(tapFire) m.Start(x, y);
      }

      FlyAway(deltaX);
      MoveColumnsCheckPass(deltaX);
      b.Move(delta, antiGravity, direction, deltaX);
    }
  }

  m.Move(delta, antiGravity, direction, deltaX);

  MoveBonus();

  if(!gameOver)
  {
    floorOffset = fmod(floorOffset + deltaX, 1/yMulValue);

    if(autoPilot) AutoPilot(delta);
    else if(antiGravity) y += delta/GRAVITY_TUNE*speed;
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
}

void Game::AutoPilot(const float delta)
{
  speed = 0;

//  autoPilot = max(autoPilot-delta, 0);
  if(delta > autoPilot) autoPilot = 0; else autoPilot -= delta;
  if(autoPilot==0)
     SoundPlayer::PlayEndOfAutoPilot();

  float d1=100.0, d2=100.0, y1=y, y2=y, x1=x, x2=x;
  for(int i=0; i<MAX_COLUMNS; i++)
  {
    Column & g = gaps[i];
    float x0 = g.GetX();
    if((x >= x0-COLUMN_HALFWIDTH-BIRD_RADIUS) && (x <= x0+COLUMN_HALFWIDTH+BIRD_RADIUS))
    {
      y = g.GetY();
      return;
    }

    if( g.GetPassed() )
      x0 += direction * (COLUMN_HALFWIDTH + BIRD_RADIUS);
    else
      x0 -= direction * (COLUMN_HALFWIDTH + BIRD_RADIUS);
    float d = fabs(x0 - x);

    if( g.GetPassed() )
    {
      if(d<=d1) 
      {
        d1 = d; x1 = x0;
        if(g.GetSolid()) y1 = y; else y1 = g.GetY();
      }
    }
    else //set next gap
    {
      if(d<=d2) 
      {
        d2 = d; x2 = x0;
        if(g.GetSolid()) y2 = y; else y2 = g.GetY();
      }
    }

  }

  float dx0 = fabs(x2 - x);
  float dx = fabs(x2 - x1);
  float dy = y2 - y1;

  if(dx > 0.00000001)
    y = y1 + dy * ((dx-dx0) / dx);
}

int Game::OnBackPressed()
{
  if(!showMenu) return 0;

  showMenu = 0;
  return 1;
}
