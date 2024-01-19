//一个简单的规律的字符串，用逗号做分割符
char *str="123,456,789,abc,";
/*
dst 保存分割好的数据的数组
str 被分割的字符串
spl 分割符
*/
int split(char dst[][5], char* str, const char* spl)
  {
  int n = 0;
  char *result = NULL;
  result = strtok(str, spl);

  while( result != NULL )
    {
      strcpy(dst[n++], result);
      result = strtok(NULL, spl);
    }
    return n;
}

void setup()
{
Serial.begin(9600);

char dst[5][5];

Serial.println("split string...");

int cnt = split(dst, str, ",");

for (int i = 0; i < cnt; i++)
Serial.println(dst[i]);

}

void loop()
{

}
