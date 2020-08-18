int inc_and_add(int & __restrict a, int & __restrict b) {
  a = 4;
  b = 5;
  return a + b;
}
