#include "mitkPolygonInteractor.h"
#include "mitkStatusBar.h"
#include <mitkInteractionConst.h>
#include <mitkPositionEvent.h>
#include <mitkOperationEvent.h>
#include <mitkStateEvent.h>
#include <mitkPointOperation.h>
#include <mitkDataTreeNode.h>
#include <mitkMesh.h>
#include "mitkAction.h"


mitk::PolygonInteractor::PolygonInteractor(const char * type, DataTreeNode* dataTreeNode)
: Interactor(type, dataTreeNode), m_PointIdCount(0), m_LineIdCount(0)
{
}

mitk::PolygonInteractor::~PolygonInteractor()
{
}

int mitk::PolygonInteractor::GetId()
{
  return m_Id;
}

//##Documentation
//##@brief makes sure, that one line is selected.
//##if no line is selected, then the next line to the given point is selected
//##if more than one line is selected, then all are deselected and the one next to the given point is selected.
//## Returns true if success, false if an error occured
bool mitk::PolygonInteractor::SelectOneLine(ITKPoint3D itkPoint)
{
 // mitk::Mesh* data = dynamic_cast<mitk::Mesh*>(m_DataTreeNode->GetData());
	//if (data == NULL)
 // {
 //   mitk::StatusBar::DisplayText("Error! Sender: PolygonInteractor; Message: Wrong type of Data!", 10000);
 //   return false;
 // }
 // 
 //// mitk::Mesh::CellDataContainerIterator it, end;
 //// end = data->GetMesh()->GetCellData()->End();
 ////	for (it = data->GetMesh()->GetCellData()->Begin(); it != end; it++)
	////{
 ////   
 //// }

 // mitk::Mesh:: cell;
 // bool ok = data->GetMesh()->GetCellData(m_Id, cell)
  return true;


}


bool mitk::PolygonInteractor::ExecuteAction(Action* action, mitk::StateEvent const* stateEvent, int objectEventId, int groupEventId)
{
  bool ok = false;//for return type bool
  
  //checking corresponding Data; has to be a Mesh or a subclass
	mitk::Mesh* data = dynamic_cast<mitk::Mesh*>(m_DataTreeNode->GetData());
	if (data == NULL)
		return false;

  
  switch (action->GetActionId())
	{
  case AcDONOTHING:
    ok = true;
	  break;
  case AcINSERTPOINT:
    //add a new point to the List of Points
    {
      mitk::PositionEvent const  *posEvent = dynamic_cast <const mitk::PositionEvent *> (stateEvent->GetEvent());
		  if (posEvent == NULL) 
        return false;

      //inserting the new interactor into the list
      //referencecounting has to be made. but so far I can see it is ok. 
      mitk::PointInteractor::Pointer pointInteractor = new mitk::PointInteractor("pointinteractor", m_DataTreeNode, m_PointIdCount);
      m_PointList->insert(PointListType::value_type(m_PointIdCount, pointInteractor));

      //Now add the point to the data
		  mitk::ITKPoint3D itkPoint;
		  mitk::vm2itk(posEvent->GetWorldPosition(), itkPoint);
      mitk::PointOperation* doOp = new mitk::PointOperation(OpINSERT, itkPoint, m_PointIdCount);
		  if (m_UndoEnabled)
		  {
        mitk::PointOperation* undoOp = new mitk::PointOperation(OpREMOVE, itkPoint, m_PointIdCount);
			  OperationEvent *operationEvent = new OperationEvent(data,
				  													doOp, undoOp,
					  												objectEventId, groupEventId);
			  m_UndoController->SetOperationEvent(operationEvent);
		  }
		  data->ExecuteOperation(doOp);
	    ++m_PointIdCount;
    }
    ok = true;
	  break;
  case AcINSERTLINE:
    {
      mitk::PositionEvent const  *posEvent = dynamic_cast <const mitk::PositionEvent *> (stateEvent->GetEvent());
		  if (posEvent == NULL) 
        return false;
      //only one line may be selected. get the line and rebuild the cell with the ID of the new 
      //point inserted between the two points of the selected line
      mitk::ITKPoint3D itkPoint;
		  mitk::vm2itk(posEvent->GetWorldPosition(), itkPoint);

      SelectOneLine(itkPoint);
      //now one line is selected
      
      //create a new LineInteractor. then connect one old LineInteractor (is selected) to the new point, 
      //the new lineInteractor to the same new point and to the other old point. With respect to undo!
    

    }
    ok = true;
    break;
  case AcCHECKGREATERZERO:
    {
      //check if the number of points is greater to zero.
      if (data->GetSize()>0)
      {
        mitk::StateEvent* newStateEvent = new mitk::StateEvent(StYES, stateEvent->GetEvent());
        this->HandleEvent( newStateEvent, objectEventId, groupEventId );
		    ok = true;
      }
      else 
      {
        mitk::StateEvent* newStateEvent = new mitk::StateEvent(StNO, stateEvent->GetEvent());
        this->HandleEvent(newStateEvent, objectEventId, groupEventId );
		    ok = true;
      }
    }
    break;
    case AcCHECKGREATERTWO:
    {
      //check if the number of points is greater to two.
      if (data->GetSize()>2)
      {
        mitk::StateEvent* newStateEvent = new mitk::StateEvent(StYES, stateEvent->GetEvent());
        this->HandleEvent( newStateEvent, objectEventId, groupEventId );
		    ok = true;
      }
      else 
      {
        mitk::StateEvent* newStateEvent = new mitk::StateEvent(StNO, stateEvent->GetEvent());
        this->HandleEvent(newStateEvent, objectEventId, groupEventId );
		    ok = true;
      }
    }
    break;


//  case AcREMOVE:
//  {
//    mitk::ITKPoint3D newPoint;
//    newPoint.Fill(0);
////critical, cause the inverse operation of removecell is not necesarily insertline. We don't know how the cell looked like when deleted.
//    PointOperation* doOp = new mitk::PointOperation(OpREMOVECELL, newPoint, m_Id);
//    if ( m_UndoEnabled )
//    {
//      LineOperation* undoOp = new mitk::LineOperation(OpINSERTLINE, m_PIdA, m_PIdB, m_Id);
//      OperationEvent *operationEvent = new OperationEvent(m_DataTreeNode->GetData(),
//                                                        doOp, undoOp,
//                                                        objectEventId, groupEventId);
//      m_UndoController->SetOperationEvent(operationEvent);
//    }
//    //execute the Operation
//    m_DataTreeNode->GetData()->ExecuteOperation(doOp);
//    ok = true;
//  }
//  break;
//  case AcREMOVEALL://remove Line and the two points
//  {
//    //removing the line
//    mitk::ITKPoint3D newPoint;
//    newPoint.Fill(0);
//    PointOperation* doOp = new mitk::PointOperation(OpREMOVECELL, newPoint, m_Id);
//    if ( m_UndoEnabled )
//    {
//      LineOperation* undoOp = new mitk::LineOperation(OpINSERTLINE, m_Id, m_PIdA, m_PIdB);
//      OperationEvent *operationEvent = new OperationEvent(m_DataTreeNode->GetData(),
//                                                        doOp, undoOp,
//                                                        objectEventId, groupEventId);
//      m_UndoController->SetOperationEvent(operationEvent);
//    }
//    //execute the Operation
//    m_DataTreeNode->GetData()->ExecuteOperation(doOp);
//
//    //now the two points. The StateMachines have to be deleted and the Data changed
//    //for undo to work, we have to store the state the machine was in when deleted
//    Operation* doOpA = new mitk::Operation(OpDELETE);
//    if ( m_UndoEnabled )
//    {
//      StateTransitionOperation* undoOpA = new mitk::StateTransitionOperation(OpUNDELETE, m_PointA->GetCurrentState());
//      OperationEvent *operationEventA = new OperationEvent(m_PointA,
//                                                        doOpA, undoOpA,
//                                                        objectEventId, groupEventId);
//      m_UndoController->SetOperationEvent(operationEventA);
//    }
//    //execute the Operation
//    m_PointA->ExecuteOperation(doOpA);
//    
//    Operation* doOpB = new mitk::Operation(OpDELETE);
//    if ( m_UndoEnabled )
//    {
//      StateTransitionOperation* undoOpB = new mitk::StateTransitionOperation(OpUNDELETE, m_PointB->GetCurrentState());
//      OperationEvent *operationEventB = new OperationEvent(m_PointB,
//                                                        doOpB, undoOpB,
//                                                        objectEventId, groupEventId);
//      m_UndoController->SetOperationEvent(operationEventB);
//    }
//    //execute the Operation
//    m_PointB->ExecuteOperation(doOp);
//
//    m_PointA = NULL;
//    m_PointB = NULL;
//
//
//    ok = true;
//  }
//  break;
  case AcMODESELECT:
    m_Mode = SMSELECTED;
    ok = true;
    break;
  case AcMODEDESELECT:
    m_Mode = SMDESELECTED;
    ok = true;
    break;
  default:
    itkWarningMacro("Message from mitkPolygonInteractor: I do not understand the Action! Check");
    return false;
  }
  return ok;
}
