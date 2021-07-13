#include <zeda/zeda.h>

zHeapClass( Tree, int );
zHeapClassMethod( Tree, int, NULL, NULL );

#if 0
void TreeNodePrint(Tree *node, int indent)
{
  printf( "%*d\n", indent, node->data );
  if( node->child[0] ) TreeNodePrint( node->child[0], indent+2 );
  if( node->child[1] ) TreeNodePrint( node->child[1], indent+2 );
}

void TreePrint(Tree *tree)
{
  if( !zTreeIsEmpty( tree ) )
    TreeNodePrint( tree->child[0], 0 );
}
#endif

int TreeComp(Tree *node1, Tree *node2, void *util)
{
  return node1->data <= node2->data ? 1 : 0;
}

#define N 10000

bool check_heap(Tree *tree)
{
  Tree *node;
  int prev;
  bool result = true;

  prev = ( node = TreeDeleteHeap( tree, TreeComp, NULL ) )->data;
  free( node );
  while( !zTreeIsEmpty( tree ) ){
    if( ( node = TreeDeleteHeap( tree, TreeComp, NULL ) )->data < prev ) result = false;
    free( node );
  }
  TreeDestroy( tree );
  return result;
}

void assert_heap(void)
{
  Tree tree;
  int i, val;

  TreeInit( &tree );
  for( i=0; i<N; i++ ){
    val = zRandI(0,10);
    TreeAddHeap( &tree, &val, TreeComp, NULL );
  }
  zAssert( zHeapClass, check_heap( &tree ) );
}

void assert_heapify(void)
{
  Tree tree;
  int i, val;

  TreeInit( &tree );
  for( i=0; i<N; i++ ){
    val = zRandI(0,10);
    TreeAddComplete( &tree, &val );
  }
  TreeHeapify( &tree, TreeComp, NULL );
  zAssert( zHeapClass (Heapify), check_heap( &tree ) );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_heap();
  assert_heapify();
  return 0;
}
