# tests/test_min_heap.py

from pydatastructs.min_heap import MinHeap

def test_insert_single():
    heap = MinHeap()
    heap.insert(10)
    assert len(heap) == 1

def test_insert_multiple():
    heap = MinHeap()
    heap.insert(10)
    heap.insert(5)
    heap.insert(15)
    heap.insert(1)
    assert len(heap) == 4

def test_remove_min():
    heap = MinHeap()
    heap.insert(10)
    heap.insert(5)
    heap.insert(1)
    assert heap.remove() == 1
    assert len(heap) == 2

def test_remove_all():
    heap = MinHeap()
    values = [10, 3, 7, 1]
    for v in values:
        heap.insert(v)

    removed = [heap.remove() for _ in range(len(values))]
    assert removed == sorted(values)
    assert len(heap) == 0

def test_remove_empty():
    heap = MinHeap()
    assert heap.remove() is None

def test_insert_and_remove_alternating():
    heap = MinHeap()
    heap.insert(20)
    assert heap.remove() == 20
    heap.insert(10)
    heap.insert(5)
    assert heap.remove() == 5
    assert heap.remove() == 10
    assert heap.remove() is None

def test_order_preserved():
    heap = MinHeap()
    for v in [9, 4, 7, 1, 2, 6]:
        heap.insert(v)

    results = [heap.remove() for _ in range(6)]
    assert results == sorted([9, 4, 7, 1, 2, 6])

def test_insert_duplicates():
    heap = MinHeap()
    for v in [5, 1, 5, 1, 5]:
        heap.insert(v)

    assert heap.remove() == 1
    assert heap.remove() == 1
    assert heap.remove() == 5
    assert heap.remove() == 5
    assert heap.remove() == 5
    assert heap.remove() is None

def test_peek_empty():
    heap = MinHeap()
    assert heap.peek() is None

def test_peek_single():
    heap = MinHeap()
    heap.insert(42)
    assert heap.peek() == 42
    assert len(heap) == 1

def test_peek_multiple():
    heap = MinHeap()
    heap.insert(10)
    heap.insert(2)
    heap.insert(15)
    assert heap.peek() == 2
    assert len(heap) == 3

def test_peek_after_remove():
    heap = MinHeap()
    heap.insert(7)
    heap.insert(3)
    heap.insert(9)
    assert heap.remove() == 3
    assert heap.peek() == 7

def test_pushpop_smaller_than_root():
    heap = MinHeap()
    for v in [10, 20, 30]:
        heap.insert(v)

    removed = heap.pushpop(5)
    assert removed == 5
    assert heap.peek() == 10

def test_pushpop_larger_than_root():
    heap = MinHeap()
    for v in [10, 20, 30]:
        heap.insert(v)

    removed = heap.pushpop(25)
    assert removed == 10
    assert heap.peek() == 20

def test_heapify_creates_valid_heap():
    values = [9, 4, 7, 1, -2, 6, 5]
    heap = MinHeap.heapify(values)

    assert len(heap) == len(values)

    sorted_out = [heap.remove() for _ in range(len(values))]
    assert sorted_out == sorted(values)

def test_heapify_empty():
    heap = MinHeap.heapify([])
    assert len(heap) == 0
    assert heap.peek() is None