#include <zeda/zeda.h>

zHeapClass( Tree, int );
zHeapClassMethod( Tree, int );

#if 0
void TreeNodePrint(Heap *node, int indent)
{
  printf( "%*d\n", indent, node->data );
  if( node->child[0] ) TreeNodePrint( node->child[0], indent+2 );
  if( node->child[1] ) TreeNodePrint( node->child[1], indent+2 );
}

void TreePrint(Heap *tree)
{
  if( !TreeIsEmpty( tree ) )
    TreeNodePrint( tree->child[0], 0 );
}
#endif

int TreeComp(Tree *node1, Tree *node2, void *util)
{
  return node1->data <= node2->data ? 1 : 0;
}

#define N 1000

int main(int argc, char *argv[])
{
  Tree tree, *node;
  int i, prev;
  bool result = true;

  zRandInit();
  TreeInit( &tree );
  for( i=0; i<N; i++ )
    TreeAddHeap( &tree, zRandI(0,10), TreeComp, NULL );

  prev = ( node = TreeDeleteHeap( &tree, TreeComp, NULL ) )->data;
  free( node );
  while( !TreeIsEmpty( &tree ) ){
    if( ( node = TreeDeleteHeap( &tree, TreeComp, NULL ) )->data < prev ) result = false;
    free( node );
  }
  TreeDestroy( &tree );
  zAssert( zHeapClass, result );
  return 0;
}
