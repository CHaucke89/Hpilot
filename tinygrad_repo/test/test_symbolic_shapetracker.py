import unittest
from tinygrad.shape.shapetracker import ShapeTracker, View
from tinygrad.shape.symbolic import Variable
from tinygrad.tensor import Tensor

class TestSymbolic(unittest.TestCase):
  def test_symbolic_st(self):
    x = Variable("x", 1, 100)
    st = ShapeTracker.from_shape((x, 3))
    assert st.shape == (x, 3)
    assert st.real_strides() == (3, 1)

  def test_expr_idxs(self):
    x = Variable("x", 1, 100)
    st = ShapeTracker.from_shape((x, 3))
    idxs = [Variable("x", 0, 100), Variable("y", 0, 100)]
    e1, e2 = st.expr_idxs(idxs)
    assert e1.render() == "((x*3)+y)"
    assert e2.render() == "1"
    st = st.permute((1, 0))
    e1, e2 = st.expr_idxs(idxs)
    assert e1.render() == "((y*3)+x)"
    assert e2.render() == "1"

  def test_cat_dim0_strides(self):
    i = Variable("i", 1, 5).bind(3)
    j = Variable("j", 1, 5).bind(3)
    k = Variable("k", 1, 5).bind(3)
    t = Tensor.rand(3, 4).reshape(i, 4).cat(Tensor.rand(3, 4).reshape(j, 4), dim=0).cat(Tensor.rand(3, 4).reshape(k, 4), dim=0)
    st = t.lazydata.st
    assert st.shape == (i+j+k, 4)
    assert st.real_strides() == (4, 1)
    t = Tensor.rand(3, 3).reshape(i, 3).cat(Tensor.rand(3, 3).reshape(i, 3), dim=0).cat(Tensor.rand(3, 3), dim=0)
    st = t.lazydata.st
    assert st.shape == (2*i+3, 3)
    assert st.real_strides() == (3, 1)

  def test_cat_dim1_strides(self):
    i = Variable("i", 1, 5).bind(4)
    j = Variable("j", 1, 5).bind(4)
    k = Variable("k", 1, 5).bind(4)
    t = Tensor.rand(3, 4).reshape(3, i).cat(Tensor.rand(3, 4).reshape(3, j), dim=1).cat(Tensor.rand(3, 4).reshape(3, k), dim=1)
    st = t.lazydata.st
    assert st.shape == (3, i+j+k)
    assert st.real_strides() == (i+j+k, 1)

class TestSymbolicVarVals(unittest.TestCase):
  def test_var_vals_empty(self):
    assert ShapeTracker.from_shape((3, 4, 5)).var_vals == {}

  def test_var_vals_shape(self):
    x = Variable("x", 1, 100).bind(3)
    assert ShapeTracker.from_shape((x, 3)).var_vals == {Variable("x", 1, 100): 3}

  def test_var_vals_offset(self):
    x = Variable("x", 1, 100).bind(3)
    st = ShapeTracker.from_shape((4, 3)).shrink(((x, x+1), (0, 3)))
    assert st.real_offset() == x * 3
    assert st.var_vals == {Variable("x", 1, 100): 3}

  def test_var_vals_mask(self):
    x = Variable("x", 1, 100).bind(3)
    view = View.create(shape=(3,4), strides=(4,1), offset=0, mask=((0, x), (0, 4)))
    st = ShapeTracker(views=(view,))
    assert st.var_vals == {Variable("x", 1, 100): 3}

  def test_var_vals_complex(self):
    x = Variable("x", 1, 100).bind(3)
    y = Variable("y", 1, 100).bind(4)
    z = Variable("z", 1, 100).bind(5)
    st = ShapeTracker.from_shape((x, 5, y)).shrink(((0, x), (z, z+1), (0, 3)))
    assert st.real_offset() == y * z
    assert st.var_vals == {Variable("x", 1, 100): 3, Variable("y", 1, 100):4, Variable("z", 1, 100): 5}

  def test_shrink_reshape(self):
    x = Variable("x", 1, 100).bind(3)
    st = ShapeTracker.from_shape((10, 10, 10)).shrink(((x, x+3), (3, 7), (2, 5)))
    st = st.reshape((3*4*3,))
    assert st.var_vals == {Variable("x", 1, 100): 3}

class TestShapeTrackerUnbind(unittest.TestCase):
  def test_view_unbind(self):
    v = Variable("v", 1, 100)
    bv = Variable("v", 1, 100).bind(3)
    assert View.create(shape=(bv, 4)).unbind() == View.create(shape=(v, 4))

  def test_reshape_unbind(self):
    v = Variable("v", 1, 100)
    bv = Variable("v", 1, 100).bind(3)
    t = Tensor.rand(3, 4).reshape(bv, 4)
    assert t.lazydata.st.unbind() == ShapeTracker((View.create(shape=(v, 4)),))

  def test_shrink_unbind(self):
    v = Variable("v", 1, 100)
    bv = Variable("v", 1, 100).bind(2)
    t = Tensor.rand(3, 4).shrink(((bv, bv+1), (0, 4)))
    assert t.lazydata.st.unbind() == ShapeTracker((View.create(shape=(1, 4), offset=4*v),))

class TestSymbolicReshape(unittest.TestCase):
  def test_reshape_into_symbols_simple(self):
    for i in range(1, 6):
      vi = Variable("i", 1, 5).bind(i)
      t = Tensor.rand(i, 4).reshape(vi, 4)
      assert t.shape == (vi, 4)
      t = Tensor.rand(i, 6).reshape(vi, 2, 3)
      assert t.shape == (vi, 2, 3)

  def test_reshape_symbols_reshape_ints(self):
    for i in range(1, 6):
      vi = Variable("i", 1, 5).bind(i)
      t = Tensor.rand(i, 4).reshape(vi, 4)
      assert t.shape == (vi, 4)
      t = t.reshape(i, 4)
      assert t.shape == (i, 4)

  def test_reshape_into_symbols_bad_shape(self):
    vi = Variable("i", 1, 10).bind(4)
    with self.assertRaises(AssertionError):
      t = Tensor.rand(4, 6).reshape(vi, 6).reshape(1, 77) # reshape to a different size new shape through symbolic shape
    with self.assertRaises(AssertionError):
      t = Tensor.rand(3, 4).reshape(3, (vi+1)) # reshape into non-Variable Node

  def test_two_symbol_reshape(self):
    for i in range(1, 6):
      for j in range(1, 6):
        vi = Variable("i", 1, 5).bind(i)
        vj = Variable("j", 1, 5).bind(j)
        t = Tensor.rand(i, j).reshape(vi, vj)
        assert t.shape == (vi, vj)
        # NOTE: this is currently not allowed
        # t = t.reshape(1, vi*vj)
        # assert t.shape == (1, vi*vj)
        t = t.reshape(vj, vi)
        assert t.shape == (vj, vi)

class TestSymbolicExpand(unittest.TestCase):
  def test_expand_into_symbols(self):
    # TODO: enfore expand only into bound variables
    vi = Variable("i", 1, 5)
    vj = Variable("j", 1, 5)
    a = Tensor([[1], [2], [3]]).expand((3, vi))
    assert a.shape == (3, vi)
    a = a.reshape(3, vi, 1).expand((3, vi, vj))
    assert a.shape == (3, vi, vj)

  def test_plus_expands_constant(self):
    for i in range(1, 6):
      vi = Variable("i", 1, 5).bind(i)
      a = Tensor.rand(3, i).reshape(3, vi)
      a = a + 1
      assert a.shape == (3, vi)

class TestSymbolicShrink(unittest.TestCase):
  def test_shrink_symbols(self):
    vi = Variable("i", 1, 5)
    t = Tensor.rand(3, 5).shrink(((0, 2), (vi, vi+1)))
    assert t.shape == (2, 1)

class TestSymbolicShapeExpr(unittest.TestCase):
  def test_symbolic_expr_idxs(self):
    # taken from symbolic shape llama
    i = Variable("i", 1, 120)
    gidx0 = Variable("gidx0", 0, i)
    lidx1 = Variable("lidx1", 0, 7)
    idx = (gidx0, lidx1, Variable.num(1))
    shape = (i+1, 8, 4)
    strides = (1, (i*4)+4, i+1)
    st = ShapeTracker((View.create(shape, strides), ))
    idx, valid = st.expr_idxs(idx)
    assert idx.render() == "((lidx1*((i*4)+4))+1+gidx0+i)"

if __name__ == '__main__':
  unittest.main()