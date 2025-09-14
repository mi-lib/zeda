#include <zeda/zeda_list.h>

zListClass( IntList,IntListCell, int );

int main(void)
{
  IntList list;
  const int size = 10;

  std::cout << "size = " << list.size << std::endl;
  for(int i=0; i<size; i++ ){
    IntListCell *cell = new IntListCell;
    cell->data = i;
    list.insertHead( cell );
  }
  std::cout << "size = " << list.size << std::endl;
  while( !list.isEmpty() ){
    IntListCell *cell = list.deleteHead();
    std::cout << " deleted: " << cell->data << std::endl;
    delete cell;
  }
  std::cout << "size = " << list.size << std::endl;
  return 0;
}
