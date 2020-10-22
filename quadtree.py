import copy

def quadtree(p, dimensions, bbox, depth, vs, us, cost, coordinate_map, ind):
    """
    Build a quadtree

    p : list
        A list of points.
        p[i] is a tuple that represents the i-th point.
        p[i][j] is the j-th coordinate of point i.
    dimensions : int
        The number of dimensions.
    bbox : list
        The bounding box.
        bbox[0] and bbox[1] should be lists of length dimensions.
        bbox[0] represents the lest-most coordinates.
        bbox[1] represents the right-most cordinates.
    depth : int
        The depth of quadtree
    vs : list (mutable)
        The list of the endpoints of quadtree.
        vs should be empty at first.
        After a function call, vs is a list of length n, and vs[i] represents the endpoint of the i-th edge.
    us : list (mutable)
        The list of the endpoints of quadtree.
        vs should be empty at first.
        After a function call, us is a list of length n, and us[i] represents the endpoint of the i-th edge.
    cost : list (mutable)
        The list of costs of quadtree.
        cost should be empty at first
        After a function call, cost is a list of length n, and cost[i] represents the cost of the i-th edge.
    coordinate_map : map (mutable)
        The map from coordinates to the node index.
        coordinate_map should be empty at first
        After a function call, coordinate_map[p[i]] represents the node index cooresponding to the point p[i].
    ind : list (mutable)
        ind should be [0] (i.e., a singleton of zero) at first.
        After a function call, ind[0] represents the number of nodes
    """
    if depth == 0:
        for c in p:
            coordinate_map[c] = ind[0]
        ind[0] += 1
        return
    cur = ind[0]
    ind[0] += 1
    np = [[] for i in range(1<<dimensions)]
    for c in p:
        cc = 0
        for coord in range(dimensions):
            if c[coord] >= (bbox[0][coord] + bbox[1][coord]) / 2:
                cc += 1 << coord
        np[cc].append(c)
    for i in range(1<<dimensions):
        if len(np[i]) >= 1:
            d = 0
            nbbox = copy.deepcopy(bbox)
            for coord in range(dimensions):
                d += (bbox[1][coord] - bbox[0][coord]) / 4
                if i & (1 << coord):
                    nbbox[0][coord] = (bbox[0][coord] + bbox[1][coord]) / 2
                else:
                    nbbox[1][coord] = (bbox[0][coord] + bbox[1][coord]) / 2
            vs.append(cur)
            us.append(ind[0])
            cost.append(d)
            quadtree(np[i], dimensions, nbbox, depth-1, vs, us, cost, coordinate_map, ind)
