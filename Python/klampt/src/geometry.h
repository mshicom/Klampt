#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <vector>
#include <map>

/** @file geometry.h
 * @brief C++ bindings for geometry modeling. */

/** @brief A 3D indexed triangle mesh class.
 *
 * Attributes:
 *
 *     vertices (SWIG vector of floats):  a list of vertices, given as
 *         a flattened coordinate list [x1, y1, z1, x2, y2, ...]
 *     indices (SWIG vector of ints): a list of triangle vertices given
 *         as indices into the vertices list, i.e., [a1,b1,c2, a2,b2,c2, ...]
 *
 * Note: because the bindings are generated by SWIG, you can access
 * the indices / vertices members via some automatically generated
 * accessors / modifiers.  In particular len(), append(), and
 * indexing via [] are useful. Some other methods like resize() are
 * also provided.  However, you CANNOT set these items via assignment. 
 *
 * Examples::
 *
 *     m = TriangleMesh()
 *     m.vertices.append(0)
 *     m.vertices.append(0)
 *     m.vertices.append(0)
 *     print len(m.vertices)  #prints 3
 *     m.vertices = [0,0,0]   #this is an error
 *     m.vertices += [1,2,3]   #this is also an error
 *
 * To get all vertices as a numpy array:
 * 
 *     verts = np.array(m.vertices).reshape((len(m.vertices)//3,3))
 *
 * To get all indices as a numpy array:
 * 
 *     inds = np.array(m.indices,dtype=np.int32).reshape((len(m.indices)//3,3))
 *
 * (Or use the convenience functions in klampt.io.numpy)
 */
struct TriangleMesh
{
  ///Translates all the vertices by v=v+t
  void translate(const double t[3]);
  ///Transforms all the vertices by the rigid transform v=R*v+t
  void transform(const double R[9],const double t[3]);

  std::vector<int> indices;
  std::vector<double> vertices;
};

/** @brief Stores a set of points to be set into a ConvexHull type. Note: 
 * These may not actually be the vertices of the convex hull; the actual
 * convex hull may be computed internally for some datatypes.
 *
 * Attributes:
 *     points (SWIG vector of floats): a list of points, given  as a flattened
 *         coordinate list [x1,y1,z1,x2,y2,...]
 */
struct ConvexHull
{
  ///Returns the # of points
  int numPoints() const; 
  ///Adds a point
  void addPoint(const double pt[3]);
  ///Retrieves a point
  void getPoint(int index,double out[3]) const;
  ///Translates all the vertices by v=v+t
  void translate(const double t[3]);
  ///Transforms all the vertices by the rigid transform v=R*v+t
  void transform(const double R[9],const double t[3]);

  std::vector<double> points;
};

/** @brief A 3D point cloud class.  
 *
 * Attributes:
 * 
 *     vertices (SWIG vector of floats): a list of vertices, given as a
 *         list [x1, y1, z1, x2, y2, ... zn]
 *     properties (SWIG vector of floats): a list of vertex properties,
 *        given as a list [p11, p21, ..., pk1,  p12, p22, ..., pk2, ...,
 *        p1n, p2n, ..., pkn] where each vertex has k properties.  The
 *        name of each property is given by the ``propertyNames`` member.
 *     propertyNames (SWIG vector of strs): a list of the names of each
 *        property
 *     settings (SWIG map of strs to strs): a general property map .
 *
 * Note: because the bindings are generated by SWIG, you can access
 * the vertices/properties/propertyName members via some automatically
 * generated accessors / modifiers.  In particular len(), append(), and
 * indexing via [] are useful. Some other methods like resize() are
 * also provided.  However, you CANNOT set these items via assignment. 
 *
 * Properties are usually lowercase but follow PCL naming convention, and often
 * include:
 *
 * - normal_x, normal_y, normal_z: the outward normal 
 * - rgb, rgba: integer encoding of RGB (24 bit int) or RGBA color (32 bit int) 
 * - opacity: opacity, in range [0,1]
 * - c: opacity, in range [0,255]
 * - r,g,b,a: color channels, in range [0,1]
 * - u,v: texture coordinate
 * 
 * Settings are usually lowercase but follow PCL naming convention, and often
 * include:
 *
 * - version: version of the PCL file, typically "0.7" 
 * - id: integer id
 * - width: the width of a structured point cloud
 * - height: the height of a structured point cloud
 * - viewpoint: "ox oy oz qw qx qy qz"
 *
 * Examples::
 * 
 *     pc = PointCloud()
 *     pc.propertyNames.append('rgb')
 *     #add 1 point with coordinates (0,0,0) and color #000000 (black)
 *     pc.vertices.append(0)
 *     pc.vertices.append(0)
 *     pc.vertices.append(0)
 *     pc.properties.append(0)
 *     print len(pc.vertices)  #prints 3
 *     print pc.numPoints()  #prints 1
 *     #add another point with coordinates (1,2,3)
 *     pc.addPoint([1,2,3])
 *     #this prints 2
 *     print pc.numPoints() 
 *     #this prints 2, because there is 1 property category x 2 points
 *     print len(pc.properties.size())
 *     #this prints 0; this is the default value added when addPoint is called
 *     print pc.getProperty(1,0) 
 *
 * To get all points as an n x 3 numpy array:
 *
 *     points = np.array(pc.vertices).reshape((pc.numPoints(),3))
 * 
 * To get all properties as a n x k numpy array:
 *
 *    properties = np.array(pc.properties).reshape((p.numPoints(),p.numProperties()))
 *
 * (Or use the convenience functions in klampt.io.numpy)
 */
struct PointCloud
{
  ///Returns the number of points
  int numPoints() const;
  ///Returns the number of properties
  int numProperties() const;
  ///Sets all the points to the given list (a 3n-list)
  void setPoints(int num,const std::vector<double>& plist);
  ///Adds a point. Sets all its properties to 0.  Returns the index.
  int addPoint(const double p[3]);
  ///Sets the position of the point at the given index to p
  void setPoint(int index,const double p[3]);
  ///Retrieves the position of the point at the given index
  void getPoint(int index,double out[3]) const;
  ///Adds a new property.  All values for this property are set to 0.
  void addProperty(const std::string& pname);
  ///Adds a new property with name pname, and sets values for this property to the given list (a n-list)
  void addProperty(const std::string& pname,const std::vector<double> & properties);
  ///Sets all the properties of all points to the given list (a kn-list)
  void setProperties(const std::vector<double>& properties);
  ///Sets property pindex of all points to the given list (a n-list)
  void setProperties(int pindex,const std::vector<double>& properties);
  ///Sets property pindex of point index to the given value
  void setProperty(int index,int pindex,double value);
  ///Sets the property named pname of point index to the given value
  void setProperty(int index,const std::string& pname,double value);
  ///Gets property pindex of point index 
  double getProperty(int index,int pindex) const;
  ///Gets the property named pname of point index
  double getProperty(int index,const std::string& pname) const;
  ///Gets property pindex of all points as an array
  void getProperties(int pindex,std::vector<double>& out) const;
  ///Gets property named pindex of all points as an array
  void getProperties(const std::string& pname,std::vector<double>& out) const;
  ///Translates all the points by v=v+t
  void translate(const double t[3]);
  ///Transforms all the points by the rigid transform v=R*v+t
  void transform(const double R[9],const double t[3]);
  ///Adds the given point cloud to this one.  They must share the same
  ///properties or else an exception is raised
  void join(const PointCloud& pc);
  ///Sets the given setting
  void setSetting(const std::string& key,const std::string& value);
  ///Retrieves the given setting
  std::string getSetting(const std::string& key) const;

  std::vector<double> vertices;
  std::vector<std::string> propertyNames;
  std::vector<double> properties;
  std::map<std::string,std::string> settings;
};

/** @brief A geometric primitive.  So far only points, spheres, segments,
 * and AABBs can be constructed manually in the Python API. 
 */
struct GeometricPrimitive
{
  void setPoint(const double pt[3]);
  void setSphere(const double c[3],double r);
  void setSegment(const double a[3],const double b[3]);
  void setAABB(const double bmin[3],const double bmax[3]);
  bool loadString(const char* str);
  std::string saveString() const;

  std::string type;
  std::vector<double> properties;
};

/** @brief An axis-aligned volumetric grid, typically a signed distance
 * transform with > 0 indicating outside and < 0 indicating inside. 
 * Can also store an occupancy grid with 1 indicating inside and 0
 * indicating outside.
 *
 * Attributes:
 *     bbox (SWIG vector of 6 doubles): contains min and max bounds
 *         (xmin,ymin,zmin),(xmax,ymax,zmax)
 *     dims (SWIG vector of  of 3 ints): size of grid in each of 3 dimensions
 *     values (SWIG vector of doubles): contains a 3D array of
 *          dims[0]*dims[1]*dims[1] values. 
 * 
 *          The cell index (i,j,k) is flattened to
 *          i*dims[1]*dims[2] + j*dims[2] + k.
 *
 *          The array index i is associated to cell index
 *          (i/(dims[1]*dims[2]), (i/dims[2]) % dims[1], i%dims[2])
 */
class VolumeGrid
{
public:
  void setBounds(const double bmin[3],const double bmax[3]);
  void resize(int sx,int sy,int sz);
  void set(double value);
  void set(int i,int j,int k,double value);
  double get(int i,int j,int k);
  void shift(double dv);

  std::vector<double> bbox; 
  std::vector<int> dims;
  std::vector<double> values; 
};

/** @brief Configures the _ext distance queries of
 * :class:`~klampt.Geometry3D`.
 *
 * The calculated result satisfies :math:`Dcalc \leq D(1+relErr) + absErr`
 * unless :math:`D \geq upperBound`, in which case Dcalc=upperBound may 
 * be returned.
 *
 * Attributes:
 * 
 *     relErr (float, optional): Allows a relative error in the reported
 *         distance to speed up computation.  Default 0.
 *     absErr (float, optional): Allows an absolute error in the reported
 *         distance to speed up computation.  Default 0.
 *     upperBound (float, optional): The calculation may branch if D exceeds
 *         this bound.
 *
 */
class DistanceQuerySettings
{
public:
  DistanceQuerySettings();
  double relErr;
  double absErr;
  double upperBound;
};

/** @brief The result from a "fancy" distance query of 
 * :class:`~klampt.Geometry3D`.
 *
 * Attributes:
 *
 *     d (float): The calculated distance, with negative values indicating
 *         penetration.  Can also be upperBound if the branch was hit.
 *     hasClosestPoints (bool):  If true, the closest point information is
 *         given in cp0 and cp1, and elem1 and elem2
 *     hasGradients (bool):  f true, distance gradient information is given
 *         in grad0 and grad1.
 *     cp1, cp2 (list of 3 floats, optional): closest points on self vs other,
 *         both given in world coordinates
 *     grad1, grad2 (list of 3 floats, optional): the gradients of the
 *         objects' signed distance fields at the closest points.  Given in
 *         world coordinates. 
 *         
 *         I.e., to move object1 to touch object2, move it in direction
 *         grad1 by distance -d.  Note that grad2 is always -grad1.
 *     elems1, elems2 (int): for compound objects, these are the
 *         element indices corresponding to the closest points.
 *
 */
class DistanceQueryResult
{
public:
  double d;
  bool hasClosestPoints;
  bool hasGradients;
  std::vector<double> cp1,cp2;
  std::vector<double> grad1,grad2;
  int elem1,elem2;
};

/** @brief The result from a contact query of :class:`~klampt.Geometry3D`.
 * The number of contacts n is variable.
 *
 * Attributes:
 *
 *     depths (list of n floats): penetration depths for each contact point. 
 *         The depth is measured with respect to the padded geometry, and must
 *         be nonnegative. A value of 0 indicates that depth cannot be 
 *         determined accurately.
 *     points1, points2 (list of n lists of floats): contact points on self vs 
 *         other,  The top level list has n entries, and each entry is a
 *         3-list expressed in world coordinates.  If an object is padded,
 *         these points are on the surface of the padded geometry.
 *     normals (list of n lists of floats): the outward-facing contact normal
 *         from this to other at each contact point, given in world
 *         coordinates.  Each entry is a 3-list, and can be a unit vector,
 *         or [0,0,0] if the the normal cannot be computed properly.
 *     elems1, elems2 (list of n ints): for compound objects, these are the
 *         element indices corresponding to each contact.
 */
class ContactQueryResult
{
public:
  std::vector<double> depths;
  std::vector<std::vector<double> > points1,points2;
  std::vector<std::vector<double> > normals;
  std::vector<int> elems1,elems2;
};

/** @brief A three-D geometry.  Can either be a reference to a
 * world item's geometry, in which case modifiers change the 
 * world item's geometry, or it can be a standalone geometry.
 *
 * There are five currently supported types of geometry:
 *
 * - primitives (GeometricPrimitive)
 * - triangle meshes (TriangleMesh)
 * - point clouds (PointCloud)
 * - volumetric grids (VolumeGrid)
 * - groups (Group)
 * - convex hulls (ConvexHull)
 * 
 * This class acts as a uniform container of all of these types.
 *
 * Each geometry stores a "current" transform, which is automatically
 * updated for world items' geometries.  The proximity queries are
 * performed with  respect to the transformed geometries (note the
 * underlying geometry is not changed, which could be computationally
 * expensive.  The query is performed, however, as though they were).
 *
 * If you want to set a world item's geometry to be equal to a standalone
 * geometry, use the set(rhs) function rather than the assignment (=)
 * operator.
 *
 * Modifiers include any setX() functions, translate(), and transform().
 *
 * Proximity queries include collides(), withinDistance(), distance(), 
 * closestPoint(), and rayCast().  For some geometry types (TriangleMesh,
 * PointCloud), the first time you perform a query, some collision
 * detection data structures will be initialized.  This preprocessing step
 * can take some time for complex geometries.
 *
 * Each object also has a "collision margin" which may virtually fatten the
 * object, as far as proximity queries are concerned. This is useful
 * for setting collision avoidance margins in motion planning. By
 * default it is zero.  (Note that this is NOT the same thing as simulation
 * body collision padding!)
 */
class Geometry3D
{
 public:
  Geometry3D();
  Geometry3D(const Geometry3D&);
  Geometry3D(const GeometricPrimitive&);
  Geometry3D(const ConvexHull&);
  Geometry3D(const TriangleMesh&);
  Geometry3D(const PointCloud&);
  Geometry3D(const VolumeGrid&);
  ~Geometry3D();
  const Geometry3D& operator = (const Geometry3D& rhs);
  ///Creates a standalone geometry from this geometry
  Geometry3D clone();
  ///Copies the geometry of the argument into this geometry.
  void set(const Geometry3D&);
  ///Returns true if this is a standalone geometry
  bool isStandalone();
  ///Frees the data associated with this geometry, if standalone 
  void free();
  ///Returns the type of geometry: TriangleMesh, PointCloud, VolumeGrid, 
  ///GeometricPrimitive, or Group
  std::string type();
  ///Returns true if this has no contents (not the same as numElements()==0)
  bool empty();
  ///Returns a TriangleMesh if this geometry is of type TriangleMesh
  TriangleMesh getTriangleMesh();
  ///Returns a PointCloud if this geometry is of type PointCloud
  PointCloud getPointCloud();
  ///Returns a GeometricPrimitive if this geometry is of type GeometricPrimitive
  GeometricPrimitive getGeometricPrimitive();
  ///Returns a ConvexHull if this geometry is of type ConvexHull
  ConvexHull getConvexHull();
  ///Returns a VolumeGrid if this geometry is of type VolumeGrid
  VolumeGrid getVolumeGrid();
  ///Sets this Geometry3D to a TriangleMesh
  void setTriangleMesh(const TriangleMesh&);
  ///Sets this Geometry3D to a PointCloud
  void setPointCloud(const PointCloud&);
  ///Sets this Geometry3D to a GeometricPrimitive
  void setGeometricPrimitive(const GeometricPrimitive&);
  ///Sets this Geometry3D to a ConvexHull
  void setConvexHull(const ConvexHull&);
  ///Sets this Geometry3D to be a convex hull of two geometries.  Note: the relative
  ///transform of these two objects is frozen in place; i.e., setting the current
  ///transform of g2 doesn't do anything to this object.
  void setConvexHullGroup(const Geometry3D& g1, const Geometry3D & g2);
  ///Sets this Geometry3D to a volumeGrid
  void setVolumeGrid(const VolumeGrid&);
  ///Sets this Geometry3D to a group geometry.  To add sub-geometries, 
  ///repeatedly call setElement() with increasing indices.
  void setGroup();
  ///Returns an element of the Geometry3D if it is a Group, TriangleMesh, or 
  ///PointCloud.  The element will be in local coordinates.
  ///Raises an error if this is of any other type.  
  Geometry3D getElement(int element);
  ///Sets an element of the Geometry3D if it is a Group, TriangleMesh, or
  /// PointCloud. The element will be in local coordinates.
  ///Raises an error if this is of any other type.  
  void setElement(int element,const Geometry3D& data);
  ///Returns the number of sub-elements in this geometry
  int numElements();

  ///Loads from file.  Standard mesh types, PCD files, and .geom files are
  ///supported.
  bool loadFile(const char* fn);
  ///Saves to file.  Standard mesh types, PCD files, and .geom files are
  ///supported.
  bool saveFile(const char* fn);
  ///Sets the current transformation (not modifying the underlying data)
  void setCurrentTransform(const double R[9],const double t[3]);
  ///Gets the current transformation 
  void getCurrentTransform(double out[9],double out2[3]);
  ///Translates the geometry data.
  ///Permanently modifies the data and resets any collision data structures.
  void translate(const double t[3]);
  ///Scales the geometry data uniformly.
  ///Permanently modifies the data and resets any collision data structures.
  void scale(double s);
  ///Scales the geometry data with different factors on each axis.
  ///Permanently modifies the data and resets any collision data structures.
  void scale(double sx,double sy,double sz);
  ///Rotates the geometry data.
  ///Permanently modifies the data and resets any collision data structures.
  void rotate(const double R[9]);
  ///Translates/rotates/scales the geometry data.
  ///Permanently modifies the data and resets any collision data structures.
  void transform(const double R[9],const double t[3]);
  ///Sets a padding around the base geometry which affects the results of
  ///proximity queries
  void setCollisionMargin(double margin);
  ///Returns the padding around the base geometry.  Default 0
  double getCollisionMargin();
  ///Returns the axis-aligned bounding box of the object. Note: O(1) time, but may not be tight
  void getBB(double out[3],double out2[3]);
  ///Returns a tighter axis-aligned bounding box of the object than getBB. Worst case O(n) time.
  void getBBTight(double out[3],double out2[3]);
  /** @brief Converts a geometry to another type, if a conversion is
   * available.  The interpretation of param depends on the type of
   * conversion, with 0 being a reasonable default.
   *
   * Available conversions are:
   *
   *   - TriangleMesh -> PointCloud.  param is the desired dispersion of
   *        the points, by default set to the average triangle diameter. 
   *        At least all of the mesh's vertices will be returned.
   *   - TriangleMesh -> VolumeGrid.  Converted using the fast marching
   *        method with good results only if the mesh is watertight.
   *        param is the grid resolution, by default set to the average
   *        triangle diameter.
   *   - TriangleMesh -> ConvexHull.  If param==0, just calculates a convex
   *        hull. Otherwise, uses convex decomposition with the HACD library.
   *   - PointCloud -> TriangleMesh. Available if the point cloud is
   *        structured. param is the threshold for splitting triangles
   *        by depth discontinuity. param is by default infinity.
   *   - PointCloud -> ConvexHull.  Converted using SOLID / Qhull.
   *   - GeometricPrimitive -> anything.  param determines the desired
   *        resolution.
   *   - VolumeGrid -> TriangleMesh.  param determines the level set for
   *         the marching cubes algorithm.
   *   - VolumeGrid -> PointCloud.  param determines the level set.
   *   - ConvexHull -> TriangleMesh. 
   *   - ConvexHull -> PointCloud.  param is the desired dispersion of the
   *         points.  Equivalent to ConvexHull -> TriangleMesh -> PointCloud
   *
   */
  Geometry3D convert(const char* type,double param=0);
  /** @brief Returns true if this geometry collides with the other
   *
   * Unsupported types:
   *
   * - VolumeGrid - TriangleMesh
   * - VolumeGrid - VolumeGrid
   * - ConvexHull - anything else besides ConvexHull
   */
  bool collides(const Geometry3D& other);
  ///Returns true if this geometry is within distance tol to other
  bool withinDistance(const Geometry3D& other,double tol);
  ///Version 0.8: this is the same as the old distance() function.
  ///
  ///Returns the distance from this geometry to the other.  If either geometry 
  ///contains volume information, this value may be negative to indicate
  ///penetration.
  double distance_simple(const Geometry3D& other,double relErr=0,double absErr=0);
  ///Returns the the distance and closest point to the input point, given in 
  ///world coordinates.  An exception is raised if this operation is not 
  ///supported with the given geometry type.
  ///
  ///The return value contains the distance, closest points, and gradients if
  ///available.
  DistanceQueryResult distance_point(const double pt[3]);
  ///A customizable version of distance_point.
  ///The settings for the calculation can be customized with relErr, absErr, 
  ///and upperBound, e.g., to break if the closest points are at least
  ///upperBound distance from one another.  
  DistanceQueryResult distance_point_ext(const double pt[3],const DistanceQuerySettings& settings);
  ///Returns the the distance and closest points between the given geometries.
  ///
  ///If the objects are penetrating, some combinations of geometry types allow
  ///calculating penetration depths:
  ///
  ///- GeometricPrimitive-GeometricPrimitive (Python-supported sub-types only)
  ///- GeometricPrimitive-TriangleMesh (surface only)
  ///- GeometricPrimitive-PointCloud
  ///- GeometricPrimitive-VolumeGrid
  ///- TriangleMesh (surface only)-GeometricPrimitive
  ///- PointCloud-VolumeGrid
  ///- ConvexHull - ConvexHull
  ///
  ///If penetration is supported, a negative distance is returned and cp1,cp2
  ///are the deepest penetrating points.
  ///
  ///Unsupported types:
  ///
  ///- GeometricPrimitive-GeometricPrimitive subtypes segment vs aabb
  ///- PointCloud-PointCloud
  ///- VolumeGrid-TriangleMesh
  ///- VolumeGrid-VolumeGrid
  ///- ConvexHull - anything else besides ConvexHull
  ///
  ///See the comments of the distance_point function
  DistanceQueryResult distance(const Geometry3D& other);
  ///A customizable version of distance.
  ///The settings for the calculation can be customized with relErr, absErr,
  ///and upperBound, e.g., to break if the closest points are at least
  ///upperBound distance from one another.  
  DistanceQueryResult distance_ext(const Geometry3D& other,const DistanceQuerySettings& settings);
  ///Returns (hit,pt) where hit is true if the ray starting at s and pointing
  ///in direction d hits the geometry (given in world coordinates); pt is
  ///the hit point, in world coordinates.
  bool rayCast(const double s[3],const double d[3],double out[3]);
  ///Returns the set of contact points between this and other.  This set
  ///is a discrete representation of the region of surface overlap, which
  ///is defined as all pairs of points within distance
  ///self.collisionMargin + other.collisionMargin + padding1 + padding2.
  ///
  ///For some geometry types (TriangleMesh-TriangleMesh,
  ///TriangleMesh-PointCloud, PointCloud-PointCloud) padding must be positive
  ///to get meaningful contact poitns and normals.
  ///
  ///If maxContacts != 0  a clustering postprocessing step is performed.
  ///
  ///Unsupported types:
  ///
  ///- GeometricPrimitive-GeometricPrimitive subtypes segment vs aabb
  ///- VolumeGrid-GeometricPrimitive any subtypes except point and sphere.
  ///  also, the results are potentially inaccurate for non-convex VolumeGrids.
  ///- VolumeGrid-TriangleMesh
  ///- VolumeGrid-VolumeGrid
  ///- ConvexHull - anything
  ///
  ContactQueryResult contacts(const Geometry3D& other,double padding1,double padding2,int maxContacts=0);
  ///Calculates the furthest point on this geometry in the direction dir.
  ///
  ///Supported types:
  ///
  ///- ConvexHull
  ///
  void support(const double dir[3], double out[3]);

  int world;
  int id;
  void* geomPtr;
};

#endif
