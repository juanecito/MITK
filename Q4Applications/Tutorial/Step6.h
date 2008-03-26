#ifndef STEP6_H
#define STEP6_H

//#include <qmainwindow.h>
#include <QWidget>
#include <mitkDataTree.h>
#include <mitkImage.h>
#include <mitkPointSet.h>

#include <itkImage.h>

#ifndef DOXYGEN_IGNORE
class Step6 : public QWidget
{
  Q_OBJECT
public:
  Step6( int argc, char* argv[], QWidget *parent=0 );
  ~Step6() {};

  virtual void Initialize();

protected:
  void Load(int argc, char* argv[]);
  virtual void SetupWidgets();

  template < typename TPixel, unsigned int VImageDimension >
    friend void RegionGrowing( itk::Image<TPixel, VImageDimension>* itkImage, Step6* step6);

  mitk::DataTree::Pointer m_Tree;
  mitk::Image::Pointer m_FirstImage;
  mitk::PointSet::Pointer m_Seeds;

  mitk::Image::Pointer m_ResultImage;
  mitk::DataTreeNode::Pointer m_ResultNode;
 
protected slots:
  virtual void StartRegionGrowing();
};
#endif // DOXYGEN_IGNORE

#endif // STEP6_H

/**
\example Step6.h
*/

