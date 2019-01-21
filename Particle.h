#define FRICTION 1

int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

class Particle
{
  public:
    void Spawn(int pos);
    void Tick(int USE_GRAVITY);
    void Kill();
    bool Alive();
    int _pos;
    int _power;
  private:
    int _life;
    int _alive;
    int _sp;
};

void Particle::Spawn(int pos){
    _pos = pos;
    _sp = random(-200, 200);
    _power = 255;
    _alive = 1;
    _life = 220 - abs(_sp);
}

void Particle::Tick(int USE_GRAVITY){
    if(_alive){
        _life ++;
        if(_sp > 0){
            _sp -= _life/10;
        }else{
            _sp += _life/10;
        }
        if(USE_GRAVITY && _pos > 500) _sp -= 10;
        _power = 100 - _life;
        if(_power <= 0){
            Kill();
        }else{
            _pos += _sp/7.0;
            if(_pos > 1000){
                _pos = 1000;
                _sp = 0-(_sp/2);
            }
            else if(_pos < 0){
                _pos = 0;
                _sp = 0-(_sp/2);
            }
        }
    }
}

bool Particle::Alive(){
    return _alive;
}

void Particle::Kill(){
    _alive = 0;
}
