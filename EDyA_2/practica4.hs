data Tree a = E | Node (Tree a) a (Tree a) deriving Show

completo :: a -> Int -> Tree a

completo x 0 = Node E x E
completo x n = let nodo = completo x (n-1) in Node nodo x nodo

balanceado :: a -> Int -> Tree a
balanceado _ 0 = E
balanceado x n
            | mod n 2 == 0 = Node (balanceado x izquierda) x (balanceado x derecha)
            | otherwise = let t = balanceado x (div (n-1) 2) in Node t x t
            where
                izquierda = div (n-1) 2
                derecha = n - 1 - izquierda

type BST = Tree 

maximo :: Ord a => Tree a -> a
maximo (Node l a E) = a
maximo (Node l a r) = maximo r

minimo :: Ord a => Tree a -> a
minimo (Node E a _) = a
minimo (Node l a r) = minimo l

checkBST :: Ord a => Tree a -> Bool
checkBST E = True
checkBST (Node l x r) = checkBST l && checkBST r && (x > maximo l) && (x < minimo r)

insertar :: Ord a => a -> BST a -> BST a
insertar x E = Node E x E
insertar x (Node l a r)
                        | x <= a = Node (insertar x l) a r
                        | otherwise = Node l a (insertar x r)


inorder :: BST a -> [a]
inorder E = []
inorder (Node l x r) = inorder l ++ [x] ++ inorder r


splitBST :: Ord a => BST a -> a -> (BST a, BST a)
splitBST E _ = (E, E)
splitBST (Node l y r) x 
                    | x == y = (Node l y E, r)
                    | x < y = let (l', r') = splitBST l x in (l', Node r' y r)
                    | otherwise = let (l', r') = splitBST l x in (Node l y l', r')

join :: Ord a => BST a -> BST a -> BST a
join E t = t
join t E = t
join (Node h1 x h2) t2 = let (l', r') = splitBST t2 x in Node (join h1 l') x (join h2 r')


-- Ejercicio 5
data Col = R | N deriving Show
data RBT a = Empt | Nodo a Col (RBT a) (RBT a) deriving Show

data T123 a = Emp | N1 a (T123 a) (T123 a)
                    | N2 a a (T123 a) (T123 a) (T123 a)
                    | N3 a a a (T123 a) (T123 a) (T123 a) (T123 a) deriving Show

trans :: RBT a -> T123 a

trans Empt = Emp
trans (Nodo a R t1 t2) = trans (Nodo a N t1 t2)
trans (Nodo x N (Nodo y R h1 h2) (Nodo z R h3 h4)) = N3 y x z (trans h1) (trans h2) (trans h3) (trans h4)
trans (Nodo x N (Nodo y R h1 h2) h3) =  N2 y x (trans h1) (trans h2) (trans h3)
trans (Nodo x N h3 (Nodo y R h1 h2)) =  N2 x y (trans h3) (trans h1) (trans h2)
trans (Nodo x N h1 h2) = N1 x (trans h1) (trans h2)

--Ejercicio 6
fromListPHeap :: Ord a => [a] -> PHeaps a
fromListPHeap [] = Empty
fromListPHeap (x:xs) = merge (Root x []) (fromListPHeap xs)
--Ejercicio 7
data PHeaps a = Empty | Root a [PHeaps a] deriving Show

isPHeap :: Ord a => PHeaps a-> Bool
isPHeap Empty = True
isPHeap (Root a hs) = isRootLeqChild a hs && areChildrenPH hs
    where
        isRootLeqChild a [] = True
        isRootLeqChild a (Empty:hs) = isRootLeqChild a hs
        isRootLeqChild a (Root x chs:hs) = a<= x && isRootLeqChild a hs
        areChildrenPH [] = True
        areChildrenPH (Empty:hs) = areChildrenPH hs
        areChildrenPH (Root x chs:hs) = isPHeap (Root x chs) && areChildrenPH hs
    
merge :: Ord a => PHeaps a -> PHeaps a -> PHeaps a

merge Empty h1 = h1
merge h1 Empty = h1
merge h1@(Root x h1s) h2@(Root y h2s)
                                | x <= y = Root x (h1s ++ [h2])
                                | otherwise = Root y (h2s ++ [h1])

insertarPHeap :: Ord a => PHeaps a -> a -> PHeaps a
insertarPHeap h1 x = merge h1 (Root x [])

concatHeaps :: Ord a => [PHeaps a] -> PHeaps a
concatHeaps [] = Empty
concatHeaps (h1 : hs) = merge h1 (concatHeaps hs)

delMin :: Ord a => PHeaps a -> Maybe (a, PHeaps a)
delMin Empty = Nothing
delMin (Root x hs) = Just (x, mergeGeneral hs)

mergeGeneral :: Ord a => [PHeaps a] -> PHeaps a
mergeGeneral [] = Empty
mergeGeneral (h:hs) = merge h (mergeGeneral hs)

testDelMin :: (Show a, Ord a) => PHeaps a -> IO ()
testDelMin h = go h
  where
    go Empty = putStrLn "Heap vacío."
    go h = case delMin h of
        Nothing -> putStrLn "Fin del heap."
        Just (x, h') -> do
            putStrLn $ "Min: " ++ show x
            go h'
