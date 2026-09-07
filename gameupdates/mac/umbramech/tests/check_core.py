"""Run focused memory, geometry, projectile, clock, and BMP regression checks."""
from pathlib import Path
import struct
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]

def function(path, signature):
    source = (root / path).read_text()
    start = source.index(signature)
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]

source = '''
#include "precompiled.hpp"
#include "tree.hpp"
#include "octree.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <thread>
AntGlobals testGlobals = {};
AntGlobals* ant_globals = &testGlobals;
struct textureImage { int width, height; unsigned char* data; };
#define UP 1
#define DOWN 2
#define DIRECTION(dir, val) dir = val
void DrawLineOfSight(float, float, float, float) {}
List* CreateList();
Node* CreateNode(int);
void DeleteNode(List*, int);
void DestroyList(List*);
Tree* CreateTree();
TreeNode* CreateTreeNode(int);
void InsertTree(TreeNode**, TreeNode*);
void InsertTest(TreeNode**, TreeNode*);
bool SearchTree(TreeNode**, TreeNode*);
int SearchTest(TreeNode**, TreeNode*);
void DeleteTree(Tree*);
Octree** GenerateOctree();
void DeleteOctree(Octree**);
'''
for file, signature in [('src/bitmap.cpp', 'int LoadBitmap_Lin('),
                        ('src/bot.cpp', 'float FindAngle('),
                        ('src/los.cpp', 'bool CheckSight('),
                        ('src/fireants.cpp', 'void ResetBullets('),
                        ('src/gldrawlib.cpp', 'unsigned long getclock(')]:
    source += function(file, signature)
source += '''
int main(int argc, char** argv) {
  List* list = CreateList();
  list->head = CreateNode(1); list->head->next = CreateNode(2);
  DeleteNode(list, 1); assert(list->head && list->head->data == 2);
  DeleteNode(list, 99); assert(list->head->data == 2);
  DeleteNode(list, 2); assert(!list->head); DestroyList(list);
  int a = 1, b = 2;
  PtrList* ptrs = CreatePtrList();
  InsertFront(ptrs, &b); InsertFront(ptrs, &a);
  DeletePtrNode(ptrs, &a); assert(ptrs->items == 1 && ptrs->head->ptr == &b);
  DeletePtrNode(ptrs, &b); assert(ptrs->items == 0 && !ptrs->head);
  DestroyPtrList(ptrs);
  for (int alternate = 0; alternate < 2; ++alternate) {
    Tree* tree = CreateTree();
    for (int value : {8, 3, 11, 1, 6}) {
      if (alternate) InsertTest(&tree->root, CreateTreeNode(value));
      else InsertTree(&tree->root, CreateTreeNode(value));
    }
    for (int value : {1, 3, 6, 8, 11, 2, 5, 12}) {
      TreeNode target = {value, nullptr, nullptr};
      bool present = value == 1 || value == 3 || value == 6 || value == 8 || value == 11;
      assert(SearchTree(&tree->root, &target) == present);
      assert(SearchTest(&tree->root, &target) == (present ? value : 0));
    }
    DeleteTree(tree);
  }
  Octree** cells = GenerateOctree();
  assert(cells[0]->max_elements == 64);
  for (int i = 0; i < 64; ++i) {
    assert(cells[i]->list && cells[i]->x_max > cells[i]->x_min);
    assert(cells[i]->y_max > cells[i]->y_min);
  }
  assert(cells[0]->x_min == -30 && cells[63]->x_max == 30);
  DeleteOctree(cells);
  assert(FindAngle(73, 0, 0, 0, 0) == 73);
  ant_globals->line_of_sight = 14;
  auto* bots = static_cast<DriverBots*>(calloc(3, sizeof(DriverBots)));
  auto& observer = bots[0]; auto& target = bots[1];
  for (int heading = 0; heading < 360; ++heading) {
    observer.heading = heading;
    target.x = -sinf(heading * PI_180) * 4;
    target.y = -cosf(heading * PI_180) * 4;
    assert(CheckSight(&observer, &target));
    target.x = -target.x; target.y = -target.y;
    assert(!CheckSight(&observer, &target));
  }
  ant_globals->max_bullets = 2;
  StaticBot bullets[2]; memset(bullets, 0x7f, sizeof(bullets));
  auto& player = bots[2]; player.bullets = bullets; player.x = 3; player.y = 4;
  ResetBullets(&player);
  for (auto& bullet : bullets) {
    assert(bullet.state == READY_STATE && bullet.max_dist == 0 && bullet.travel == 0);
    assert(bullet.position[0] == 3 && bullet.position[2] == 4);
  }
  free(bots);
  const auto start = getclock();
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  assert(getclock() >= start);
  textureImage image = {};
  assert(LoadBitmap_Lin(argv[1], &image));
  assert(image.width == 1 && image.height == 2);
  assert(image.data[0] == 255 && image.data[1] == 0 && image.data[2] == 0);
  assert(image.data[3] == 0 && image.data[4] == 255 && image.data[5] == 0);
  free(image.data); image = {};
  for (int i = 2; i < argc; ++i) {
    assert(!LoadBitmap_Lin(argv[i], &image)); assert(!image.data);
  }
}
'''
with tempfile.TemporaryDirectory(prefix='umbra-core-') as directory:
    directory = Path(directory)
    header = struct.pack('<2sIHHI', b'BM', 62, 0, 0, 54)
    header += struct.pack('<IiiHHIIiiII', 40, 1, 2, 1, 24, 0, 8, 0, 0, 0, 0)
    valid = header + bytes([0, 0, 255, 0, 0, 255, 0, 0])
    fixtures = [valid, b'BM', valid[:-1], b'XX' + valid[2:], valid[:18] + struct.pack('<i', -1) + valid[22:]]
    paths = []
    for index, contents in enumerate(fixtures):
        path = directory / f'{index}.bmp'
        path.write_bytes(contents)
        paths.append(str(path))
    cpp = directory / 'core.cpp'
    binary = directory / 'core'
    cpp.write_text(source)
    subprocess.run(['clang++', '-std=c++17', '-Wno-deprecated-declarations',
                    '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                    '-I' + str(root / 'include'), str(cpp),
                    *[str(root / 'src' / name) for name in ('list.cpp', 'plist.cpp', 'tree.cpp', 'octree.cpp')],
                    '-o', str(binary)], check=True)
    subprocess.run([str(binary), *paths], check=True)
print('Core regression checks passed with AddressSanitizer and UndefinedBehaviorSanitizer.')
