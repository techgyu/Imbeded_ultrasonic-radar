class Node:
    def __init__(self, x, y, angle):
        self.x = x  # x 좌표
        self.y = y  # y 좌표
        self.angle = angle  # 서보 모터의 각도
        self.Transparency = 100  # 투명도
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    # 연결 리스트의 끝에 노드를 추가하는 메서드
    def append(self, x, y, angle):
        new_node = Node(x, y, angle)
        if not self.head:
            self.head = new_node
            return
        last_node = self.head
        while last_node.next:
            last_node = last_node.next
        last_node.next = new_node

    # 연결 리스트에서 특정 값을 제거하고 다시 이어주는 메서드
    def remove(self, value):
        current = self.head
        if current is None:
            return

        # 헤드 노드를 제거해야 할 경우
        if current.x == value.x and current.y == value.y and current.angle == value.angle:
            self.head = current.next
            current = None
            return

        prev = None
        while current:
            if current.x == value.x and current.y == value.y and current.angle == value.angle:
                break
            prev = current
            current = current.next

        if current is None:
            print("Node not found")
            return

        prev.next = current.next
        current = None

    # 연결 리스트를 출력하는 메서드
    def display(self):
        current = self.head
        while current:
            print(f"({current.x}, {current.y}, {current.angle}) ", end="-> ")
            current = current.next
        print("None")


# 연결 리스트 테스트
if __name__ == "__main__":
    # 연결 리스트 인스턴스 생성
    linked_list = LinkedList()

    # 데이터 추가
    linked_list.append(1, 2, 45)
    linked_list.append(3, 4, 90)
    linked_list.append(5, 6, 135)
    linked_list.append(7, 8, 180)

    # 연결 리스트 출력
    print("Original Linked List:")
    linked_list.display()

    # 특정 노드 제거 및 새로 연결
    node_to_remove = Node(3, 4, 90)
    linked_list.remove(node_to_remove)

    # 제거 후 연결 리스트 출력
    print("\nLinked List after removing node (3, 4, 90):")
    linked_list.display()
