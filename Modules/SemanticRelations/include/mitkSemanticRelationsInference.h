/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef MITKSEMANTICRELATIONSINFERENCE_H
#define MITKSEMANTICRELATIONSINFERENCE_H

#include <MitkSemanticRelationsExports.h>

// semantic relations module
#include "mitkSemanticTypes.h"

// mitk core
#include <mitkDataNode.h>

namespace mitk
{
  /**
  * @brief  The API provides functions to query image relations and instances
  *         that are helpful during follow-up examination, like control-points (time period),
  *         types of the images or lesions that may be visible on multiple images.
  *
  *   The class is able to generate IDs from given data nodes using DICOM information.
  *   These IDs are used to identify the corresponding instances of a specific case.
  *   The case can also be directly identified by the given case ID.
  *
  *   In order for most functions to work the case ID has to be used as a parameter.
  *   If not, these functions do nothing.
  */
  namespace SemanticRelationsInference
  {

    /************************************************************************/
    /* functions to get instances / attributes                              */
    /************************************************************************/
    /**
    * @brief Return a vector of lesion classes that are currently available for the given case.
    *
    * @param caseID   The current case identifier is defined by the given string.
    * @return         A vector of lesion classes.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::LesionClassVector GetAllLesionClassesOfCase(const SemanticTypes::CaseID& caseID);
    /**
    * @brief  Return the lesion that is defined by the given segmentation data.
    *
    * @pre    The given segmentation data node has to be valid (!nullptr).
    * @throw  SemanticRelationException, if the given segmentation data node is invalid (==nullptr).
    * @pre    The segmentation data node has to represent a lesion. If not, the retrieved lesion will be empty, which leads to an exception.
    * @throw  SemanticRelationException, if the segmentation does not represent an existing lesion (this can be checked via 'IsRepresentingALesion').
    *
    * @param segmentationNode   The segmentation identifier is extracted from the given data node.
    * @return                   The represented lesion.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::Lesion GetRepresentedLesion(const DataNode* segmentationNode);
    /**
    * @brief  Check if the given segmentation refers to an existing lesion instance.
    *         This function can be used before calling 'GetRepresentedLesion' in order to avoid a possible exception.
    *
    * @param segmentationNode   The segmentation identifier is extracted from the given data node.
    * @return                   True, if the segmentation refers to an existing lesion; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool IsRepresentingALesion(const DataNode* segmentationNode);
    /**
    * @brief  Check if the segmentation identified by the given segmentation ID refers to an existing lesion instance.
    *         This function can be used before calling 'GetRepresentedLesion' in order to avoid a possible exception.
    *
    * @param caseID             The current case identifier is defined by the given string.
    * @param segmentationID     The segmentation node identifier is defined by the given string.
    * @return                   True, if the segmentation refers to an existing lesion; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool IsRepresentingALesion(const SemanticTypes::CaseID& caseID, const SemanticTypes::ID& segmentationID);
    /**
    * @brief Check if the given lesion is present on the segmentation identified by the given segmentation ID.
    *
    * @param caseID             The current case identifier is defined by the given string.
    * @param lesion             A Lesion with a UID that identifies the corresponding lesion instance.
    * @param segmentationID     The segmentation node identifier is defined by the given string.
    * @return                   True, if the lesion is present on the segmentation identified by the given segmentation ID; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool IsLesionPresentOnSegmentation(const SemanticTypes::CaseID& caseID, const SemanticTypes::Lesion& lesion, const SemanticTypes::ID& segmentationID);
    /**
    * @brief Check if the given data node exists in the relation storage.
    *        The function receives the case- and the node-ID from the DICOM tags of the node itself.
    *        It uses node predicates to decide if the node is an image or a segmentation node and searches
    *        through the corresponding relations.
    *
    * @param dataNode    A data node to check.
    * @return            True, if the data node exists; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool InstanceExists(const DataNode* dataNode);
    /**
    * @brief  Check if the given lesion instance exists.
    *         This function can be used before calling 'GetAllSegmentationsOfLesion' in order to avoid a possible exception.
    *         This function can be used before calling 'AddLesionInstance' in order to avoid a possible exception.
    *
    * @param caseID   The current case identifier is defined by the given string.
    * @param lesion   A Lesion with a UID that identifies the corresponding lesion instance.
    * @return         True, if the lesion instance exists; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool InstanceExists(const SemanticTypes::CaseID& caseID, const SemanticTypes::Lesion& lesion);
    /**
    * @brief  Return a vector of all segmentation IDs that identify segmentations that define the given lesion.
    *         These segmentations don't have to be linked to the same image.
    *         If the lesion is not referred to by any segmentation, an empty vector is returned.
    *
    * @pre    The UID of the lesion has to exist for a lesion instance.
    * @throw  SemanticRelationException, if UID of the lesion does not exist for a lesion instance (this can be checked via 'InstanceExists').
    *
    * @param caseID   The current case identifier is defined by the given string.
    * @param lesion   A Lesion with a UID that identifies the corresponding lesion instance.
    * @return         A vector of IDs identifying segmentations that define the given lesion.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::IDVector GetAllSegmentationIDsOfLesion(const SemanticTypes::CaseID& caseID, const SemanticTypes::Lesion& lesion);
    /**
    * @brief  Return the control point of a data node.
    *         If the data node is not linked to a control point or the data node refers to a non-existing control point,
    *         a control point with an empty UID is returned.
    *
    * @pre    The given image data node has to be valid (!nullptr).
    * @throw  SemanticRelationException, if the given image data node is invalid (==nullptr).
    *
    * @param dataNode   The current case identifier is extracted from the given data node, which contains DICOM information about the case.
    * @return           The control point of the given data node.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ControlPoint GetControlPointOfImage(const DataNode* dataNode);
    /**
    * @brief  Check if the given control point instance exists.
    *         This function can be used before calling 'GetAllDataOfControlPoint' in order to avoid a possible exception.
    *         This function can be used before adding, linking and unlinking control points to avoid a possible exception.
    *
    * @param caseID         The current case identifier is defined by the given string.
    * @param controlPoint   A control point with a UID that identifies the corresponding control point instance.
    * @return               True, if the control point instance exists; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool InstanceExists(const SemanticTypes::CaseID& caseID, const SemanticTypes::ControlPoint& controlPoint);
    /**
    * @brief  Return a vector of all examination periods nodes that are valid for the given case.
    *
    * @param caseID         The current case identifier is defined by the given string.
    * @return               A vector of examination periods.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::ExaminationPeriodVector GetAllExaminationPeriodsOfCase(const SemanticTypes::CaseID& caseID);
    /**
    * @brief  Check if the given examination period instance exists.
    *         This function can be used before calling 'AddExaminationPeriod' in order to avoid a possible exception.
    *
    * @param caseID               The current case identifier is defined by the given string.
    * @param examinationPeriod    An examination period with a UID that identifies the corresponding examination period instance.
    * @return                     True, if the examination period instance exists; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool InstanceExists(const SemanticTypes::CaseID& caseID, const SemanticTypes::ExaminationPeriod& examinationPeriod);
    /**
    * @brief  Return a vector of all information types that are valid for the given case.
    *
    * @param caseID     The current case identifier is defined by the given string.
    * @return           A vector of information types.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::InformationTypeVector GetAllInformationTypesOfCase(const SemanticTypes::CaseID& caseID);
    /**
    * @brief  Return the information type of the given image.
    *         If the image does not contain any information type, an empty information type is returned.
    *
    * @pre    The given image data node has to be valid (!nullptr).
    * @throw  SemanticRelationException, if the given image data node is invalid (==nullptr).
    *
    * @param imageNode    The current case identifier is extracted from the given data node, which contains DICOM information about the case.
    * @return             The information type of the given data node.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::InformationType GetInformationTypeOfImage(const DataNode* imageNode);
    /**
    * @brief  Return the information type of the given image.
    *         If the image does not contain any information type, an empty information type is returned.
    *
    * @param caseID     The current case identifier is defined by the given string.
    * @param imageID    The image node identifier is defined by the given string.
    * @return           The information type of the image node, defined by the image node ID.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::InformationType GetInformationTypeOfImage(const SemanticTypes::CaseID& caseID, const SemanticTypes::ID& imageID);
    /**
    * @brief  Return a vector of all image IDs that identify images that are of the given information type.
    *         If the information type is not used by any image, an empty vector is returned.
    *
    * @pre    The information type has to exist.
    * @throw  SemanticRelationException, if the information type does not exist (this can be checked via 'InstanceExists').
    *
    * @param caseID             The current case identifier is defined by the given string.
    * @param informationType    An information type.
    * @return                   A vector of IDs identifying images that are of the given information type.
    */
    MITKSEMANTICRELATIONS_EXPORT SemanticTypes::IDVector GetAllImageIDsOfInformationType(const SemanticTypes::CaseID& caseID, const SemanticTypes::InformationType& informationType);
    /**
    * @brief  Check if the given information type exists.
    *         This function can be used before calling 'GetAllDataOfInformationType' in order to avoid a possible exception.
    *
    * @param caseID             The current case identifier is defined by the given string.
    * @param informationType    An information type.
    * @return                   True, if the information type exists; false otherwise.
    */
    MITKSEMANTICRELATIONS_EXPORT bool InstanceExists(const SemanticTypes::CaseID& caseID, const SemanticTypes::InformationType& informationType);
    /**
    * @brief  Return a vector of all CaseIDs that are currently available.
    *
    * @return       A vector of CaseIDs as strings.
    */
    MITKSEMANTICRELATIONS_EXPORT std::vector<SemanticTypes::CaseID> GetAllCaseIDs();
    /**
    * @brief Remove all control points from the storage that are not referenced by any image anymore.
    *        This might happen if an image has been removed (and unlinked from the corresponding control point)
    *        or if the user sets a new control point for an image manually in the GUI.
    *
    * @param caseID   The current case identifier is defined by the given string.
    */
    void ClearControlPoints(const SemanticTypes::CaseID& caseID);

  } // namespace SemanticRelationsInference
} // namespace mitk

#endif // MITKSEMANTICRELATIONSINFERENCE_H
