
class Point
{
  public float x;
  public float y;
  public float z;
  
  public Point(float _x, float _y, float _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }
  
  public void ToString()
  {
    System.out.println(x + " " + " " + z);
  }
}

class Buffer
{
  private int buffer[];
  private int width;

  public Buffer(int width)
  {
    this.width = _width;
    this.buffer = new int[_width * _width];
  }

  public void Put(int _x, int _y, int _value)
  {
    this.buffer[(_y * width) + _x] = _value;
  }

  public void Print()
  {
    for(int k = 0; k < WIDTH; k++)
    {
      for(int l = 0; l < WIDTH; l++)
      {
        System.out.println(buffer[(k * width) + l]);
      }
      System.out.println();
    }
  }
}

void Setup()
{

}

void Draw()
{

}
