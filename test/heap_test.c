#include <zeda/zeda.h>

zHeapClass( Heap, int );
zHeapClassMethod( Heap, int );

#if 0
void HeapNodePrint(Heap *node, int indent)
{
  printf( "%*d\n", indent, node->data );
  if( node->child[0] ) HeapNodePrint( node->child[0], indent+2 );
  if( node->child[1] ) HeapNodePrint( node->child[1], indent+2 );
}

void HeapPrint(Heap *tree)
{
  if( !HeapIsEmpty( tree ) )
    HeapNodePrint( tree->child[0], 0 );
}
#endif

int HeapComp(Heap *node1, Heap *node2, void *util)
{
  return node1->data <= node2->data ? 1 : 0;
}

#define N 1000

int main(int argc, char *argv[])
{
  Heap tree, *node;
  int i, prev;
  bool result = true;

  zRandInit();
  HeapInit( &tree );
  for( i=0; i<N; i++ )
    HeapAdd( &tree, zRandI(0,10), HeapComp, NULL );

  prev = ( node = HeapDelete( &tree, HeapComp, NULL ) )->data;
  free( node );
  while( !HeapIsEmpty( &tree ) ){
    if( ( node = HeapDelete( &tree, HeapComp, NULL ) )->data < prev ) result = false;
    free( node );
  }
  HeapDestroy( &tree );
  zAssert( zHeapClass, result );
  return 0;
}
