import Data.Map as M
import Heap as H
-- ************************************************************************* --

-- ************************************************************************* --

-- 1) --

data HTree = Leaf Char Int | Node HTree HTree Int

instance Eq HTree where
    (Leaf c1 n1) == (Leaf c2 n2) = (c1 == c2) && (n1 == n2)
    (Node left1 right1 n1) == (Node left2 right2 n2) = (left1 == left2) && (right1 == right2) && (n1 == n2) 
    _ == _ = False

instance Ord HTree where
    compare (Leaf _ n1) (Leaf _ n2) = compare n1 n2
    compare (Leaf _ n1) (Node _ _ n2) = compare n1 n2
    compare (Node _ _ n1) (Leaf _ n2) = compare n1 n2
    compare (Node _ _ n1) (Node _ _ n2) = compare n1 n2 

type FreqMap = Map Char Int

-- 2) --

buildFreqMap :: String -> FreqMap
buildFreqMap "" = fromList []
buildFreqMap (c:cs) = insertWith (+) c 1 (buildFreqMap cs)

-- 3) --

weight :: HTree -> Int
weight (Leaf _ n) = n
weight (Node _ _ n) = n
 
buildHeap :: [(Char,Int)] -> H.Heap HTree
buildHeap [] = H.empty
buildHeap ((k,a):ms) = H.insert (Leaf k a) (buildHeap ms)

buildHTree' :: H.Heap HTree -> HTree
buildHTree' h = if H.isEmpty (H.deleteMin h) then (H.findMin h)
    else
        buildHTree' (H.insert (Node x' y' ((weight x') + (weight y'))) y) 
            where x = (H.deleteMin h)
                  y = (H.deleteMin x)
                  x' = H.findMin h
                  y' = H.findMin x

buildHTree :: FreqMap -> HTree
buildHTree m = buildHTree' (buildHeap (M.toList m))
-- 4) --

