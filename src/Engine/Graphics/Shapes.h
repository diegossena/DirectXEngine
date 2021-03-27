class outside
{
public:
  class nested
  {
  public:
    static int x;
    static int y;
    int f();
    int g();
  };
};
int outside::nested::x = 5;
//void Graph::square

void testFunc()
{
  outside test;
}