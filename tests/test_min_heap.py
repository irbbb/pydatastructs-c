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
