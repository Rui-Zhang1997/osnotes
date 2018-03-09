#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// DYNAMIC LINKED LIST CODE
typedef struct QBlock {
    int* block; // pointer to start of block
    int fillCnt; // number of elements inserted into block
    struct QBlock* nBlock; // next block
    struct QBlock* pBlock; // previous block
} QBlock;

typedef struct Queue {
    QBlock* ROOT_BLOCK; // first block
    int* currBlock; // current block that we are inserting on [xxxx] -> [xxxx] -> [x] <== currBlock
    int BCnt; // total number of blocks
    int B_SIZE; // block size
    bool (*cmp) (int*, int*);
} Queue;

QBlock *newBlock(QBlock* p, int size) {
    QBlock* b = (QBlock*)calloc(1, sizeof(QBlock));
    b->block = (int*)calloc(size, sizeof(int));
    if (p != NULL) {
        p->nBlock = b;
    }
    b->fillCnt = 0;
    b->pBlock = p;
    b->nBlock = NULL;
    return b;
}

bool cmp(int* child, int* parent) { // integer max heap
    return *child < *parent;
}

Queue* initQueue(int size) { // size: size of each block
    Queue* q = (Queue*)calloc(1, sizeof(Queue));
    q->ROOT_BLOCK = newBlock(NULL, size);
    q->currBlock = q->ROOT_BLOCK;
    q->BCnt = 1;
    q->B_SIZE = size;
    q->cmp = &cmp;
    return q;
}

void addBlock(Queue* q) { // adds a block to array
    QBlock* nb = newBlock(q->currBlock, q->B_SIZE);
    q->currBlock = nb;
    q->BCnt += 1;
}

void insertToBlock(QBlock* b, int v) { // insert element to block
    b->block[fillCnt] = v;
    b->fillCnt += 1;
}

void insertItem(Queue* q, int v) {
    if (q->currBlock->fillCnt == q->size) { // current block has been filled
        addBlock(q);
    }
    insertToBlock(q->currBlock, v);
}

int* getItemRef(Queue* q, int index) { // returns the reference to the element
    QBlock* blk = q->ROOT_BLOCK;
    int SZ = q->B_SIZE;
    while (index > SZ) {
        blk = blk->nBlock;
        index -= SZ;
    }
    return blk+SZ;
}

int** getMultiRefs(Queue* q, int* lst, int sz) { // get multiple references. list index must be in ascending order
    int** refs = (int**)calloc(sz, sizeof(int*))
    int refpt = 0;
    int SZ = q->B_SIZE;
    QBlock* blk = q->ROOT_BLOCK;
    int cIndex = 0;
    while (cIndex < sz && blk != NULL) {
        if (lst[cIndex] < (SZ * (refpt + 1))) { // index is within this block
            refs[cIndex] = blk->block + (lst[cIndex] % SZ);
            cIndex += 1;
        }
        // advancing to next block
        refpt += 1;
        blk = blk->nBlock;
    }
    return refs;
}

int indexOf(Queue* q, int v) {
    QBlock* blk = q->ROOT_BLOCK;
    int SZ = q->B_SIZE;
    int ret = 0;
    while (blk != NULL) {
        int i;
        for (i = 0; i < blk->fillCnt; i++) {
            if (blk->block[i] == v) {
                return ret + i;
            }
        }
        blk = blk->nBlock;
        ret += SZ;
    }
    return -1;
}

int getItem(Queue* q, int index) { // gets the int value instead of the reference
    return *getItemRef(q, index);
}

int getItemCount(Queue* q) {
    return q->B_SIZE * q->BCnt + q->currBlock->fillCnt;
}

int deleteItem(Queue* q, int index) { // delete item from array
    QBlock* blk = q->ROOT_BLOCK;
    int SZ = q->B_SIZE;
    while (index > SZ) {
        blk = blk->nBlock;
        index -= SZ;
    }
    int ret = blk->block[index];
    int sv = index;
    while (blk != NULL) {
        if (blk->fillCnt == 0) { // nothing in this block to begin with
            q->currBlock = blk->pBlock; // setting currBlock to previous block, since last element of previous block is empty
            break;
        }
        if (blk->fillCnt == 1) { // only a single element, so shifted to the previous block already
            blk->fillCnt = 0; // this block has no elements
        }
        int i;
        for (i = sv + 1; i < blk->fillCnt; i++) {
            blk->block[i-1] = blk->block[i];
        }
        if (blk->fillCnt == SZ && blk->nBlock->fillCnt > 0) { // still more to shift
            blk->block[i-1] = blk->nBlock[0];
        }
        blk = blk->nBlock;
        sv = 0;
    }
    return ret;
}

void swap(Queue* q, int p1, int p2) { // swap two items
    int pts[] = {p1, p2};
    int** refs = getMultiRefs(q, pts, 2);
    int tmp = *refs[0];
    *refs[0] = *refs[1];
    *refs[1] = tmp;
}

int* getHead(Queue* q) {
    return q->ROOT_BLOCK;
}

int* getTail(Queue* q) {
    return q->currBlock->block + q->fillCnt - 1;
}

int getTailIndex(Queue* q) { // returns the index of the tail element
    return q->BCnt * q->B_SIZE + q->currBlock->fillCnt-1;
}

int getIndexWithPtr(Queue* q, int* i) {
    int SZ = q->B_SIZE;
    QBlock* blk = q->ROOT_BLOCK;
    int blksTraveled = 0;
    while(!!((i - blk->block) < SZ && (i - blk->block) >= 0)) {
        blk = blk->nBlock;
        blksTraveled += 1;
    }
    return blksTraveled + (i - blk->block);
}

void destroy(Queue* q) {
    Queue* blk = q->ROOT_BLOCK;
    Queue* next = blk->nBlock;
    while (blk != NULL) {
        free(blk->block);
        free(blk);
        blk = next;
        next = blk->nBlock;
    }
}

/* Theoretical, do not use unless needed. Probably does not work.
// swap multiple indices at once.
// indexList: list of indices to swap in ascending order
// indexOrder: order to swaps from indexList
// e.g. swapping 5 -> 12 -> 8 -> 2 -> 5 (back to start)
// [2, 5, 8, 12] <== indexList
// [3, 2, 0, 1] <== indexOrder
void swapN(Queue* q, int* indexList, int* indexOrder, int sz) {
    int** refs = getMultiRefs(q, indexList, sz);
    int i;
    int tmp = *refs[indexList[indexOrder[sz-1]]];
    for (i = 1; i < sz; i++) {
        *refs[indexList[indexOrder[i-1]]] = *refs[indexList[indexOrder[i]]];
    }
    *refs[indexList[indexOrder[sz-1]]] = tmp;
}*/

// The code for the queue
int peek(Queue* q) {
    return *(q->ROOT_BLOCK->block);
}

void insert(Queue* q, int v) {
    insertItem(q, v);
    int qsize = getItemCount(q);
    int refCnt = log2(qsize) + 1;
    int tailI = getTailIndex(q);
    int* positions = (int*)calloc(refCnt, sizeof(int));
    int i;
    int p = 1;
    for (i = 0; i < refCnt; i++) {
        positions[i] = tailI;
        tailI /= 2;
    }
    int** itemRefs = getMultiRefs(q, positions, refCnt);
    for (i = 0; i < refCnt-1; i++) {
        if (!q->cmp(*itemRefs[i], *itemRefs[i+1])) {
            swap(q, itemRefs[i], itemRefs[i+1]);
        } else {
            break;
        }
    }
}

void downShift(Queue* q) {
    int i;
    (q->p)--;
}

int pop(Queue* q) {
    int ret = *(q->block);
    downShift(q);
    return ret;
}

int main() {
    int N = 10;
    Queue* q = initQueue(N);
    for (int i = 0; i < N; i++) {
        insert(q, i);
    }

    for (int i = 0; i < N; i++) {
        printf("%d\n", pop(q));
    }
    return 0;
}
