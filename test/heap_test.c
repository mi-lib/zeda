#include <zeda/zeda.h>

zHeapClass( Tree, int );
zHeapClassMethod( Tree, int, NULL, NULL );

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

static Tree *__TreeNodeAddComplete(Tree *parent, int id, Tree *node, Tree *node_new, uint mask, uint totalsize){
  uint cid;
  cid = mask & totalsize ? 1 : 0;
  if( mask == 1 )
    node->child[cid] = node_new;
  else
    __TreeNodeAddComplete( node, cid, node->child[cid], node_new, mask >> 1, totalsize );
  return node_new;
}

Tree *TreeAddComplete(Tree *tree, int val){
  Tree *np_new;
  uint mask;
  if( !( np_new = TreeNodeAlloc( &val ) ) ) return NULL;
  if( ++tree->size == 1 ){
    tree->child[0] = np_new;
    return tree->child[0];
  }
  mask = __TreeInitHeapMask( tree );
  return __TreeNodeAddComplete( tree, 0, tree->child[0], np_new, mask, tree->size );
}

int TreeComp(Tree *node1, Tree *node2, void *util)
{
  return node1->data <= node2->data ? 1 : 0;
}

#define N 1000

bool check_heap(Tree *tree)
{
  Tree *node;
  int prev;
  bool result = true;

  prev = ( node = TreeDeleteHeap( tree, TreeComp, NULL ) )->data;
  free( node );
  while( !TreeIsEmpty( tree ) ){
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
  int i;

  TreeInit( &tree );
  for( i=0; i<N; i++ )
    TreeAddComplete( &tree, zRandI(0,10) );
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
