#ifndef STEP6_H
#define STEP6_H

#include <qmainwindow.h> 
#include <mitkDataTree.h>
#include <mitkImage.h>
#include <mitkPointSet.h>

#include <itkImage.h>

class Step6 : public QMainWindow
{
  Q_OBJECT
public:
  Step6( int argc, char* argv[], QWidget *parent=0, const char *name=0 );
  ~Step6() {};

  virtual void Initialize();

protected:
  void Load(int argc, char* argv[]);
  virtual void SetupWidgets();

  template < typename TPixel, unsigned int VImageDimension > 
  void RegionGrowing( itk::Image<typename TPixel, VImageDimension>* itkImage, mitk::Geometry3D* geometry);

  mitk::DataTree::Pointer m_Tree;
  mitk::Image::Pointer m_FirstImage;
  mitk::PointSet::Pointer m_Seeds;

  mitk::Image::Pointer m_ResultImage;
  mitk::DataTreeNode::Pointer m_ResultNode;
  QWidget* m_TopLevelWidget;
protected slots:
  virtual void StartRegionGrowing();
};
#endif // STEP6_H
