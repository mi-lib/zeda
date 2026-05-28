#include <zeda/zeda.h>

ZEDA_DEF_HEAP_CLASS( ,Tree, int )
ZEDA_DEF_HEAP_CLASS_METHOD( Tree, int, NULL, NULL )

int TreeComp(Tree *node1, Tree *node2, void *util)
{
  return node1->data <= node2->data ? 1 : 0;
}

#define N 10000

bool check_heap(Tree *tree)
{
  Tree *node = tree->get( TreeComp, NULL );
  int prev = node->data;
  free( node );
  bool result = true;
  while( !tree->isEmpty() ){
    if( ( node = tree->get( TreeComp, NULL ) )->data < prev ) result = false;
    free( node );
  }
  tree->destroy();
  return result;
}

void assert_heap(void)
{
  Tree tree;
  for(int i=0; i<N; i++ ){
    int val = zRandI(0,10);
    tree.add( &val, TreeComp, NULL );
  }
  zAssert( C++::zHeap.add, check_heap( &tree ) );
}

void assert_heapify(void)
{
  Tree tree;
  for(int i=0; i<N; i++ ){
    int val = zRandI(0,10);
    tree.addComplete( &val );
  }
  tree.heapify( TreeComp, NULL );
  zAssert( C++::zHeap.heapify, check_heap( &tree ) );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_heap();
  assert_heapify();
  return 0;
}
