/*
 * KDots
 * Copyright(c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "dottable.hpp"
#include <KMessageBox>
#include <KLocalizedString>
#include <KDebug>
#include "graph.hpp"
#include "polygonfinder.hpp"
#include "stepqueue.hpp"

namespace KDots
{
  DotTable::DotTable(const GameConfig& config, std::shared_ptr<StepQueue> step_queue, QObject *parent)
    : QObject(parent)
    , m_graph(new Graph(config.m_width, config.m_height))
    , m_steps(step_queue)
    , m_config(config)
  {
  }
  
  const GameConfig& DotTable::gameConfig() const
  {
    return m_config;
  }

  void DotTable::pushPoint(const Point& point)
  {
    Graph& graph = *m_graph;
    GraphPoint& currentPoint = graph[point];

    if(currentPoint.owner() != Owner::NONE || currentPoint.isCaptured())
      return;
    
    const Owner current = m_steps->getCurrentOwner();

    currentPoint.setOwner(current);

    m_steps->addPoint(point);

    PolygonFinder findPolygon(graph, current);

    //O(n)
    const PolyList& polyList = findPolygon(point);

    const auto& points = m_steps->getPoints(StepQueue::other(current));
    if(points.empty() || polyList.empty())
    {
      continueStep();
      emit nextPlayer(point);
      return;
    }
    
    const Owner otherOwner = StepQueue::other(current);
    
    const auto& otherOwnerPoints = m_steps->getPoints(otherOwner);
    for(const Point& p : otherOwnerPoints)
    {
      GraphPoint& gpoint = graph[p];
      if(gpoint.isCaptured())
        continue;
      
      for(const Polygon_ptr& polygon : polyList)
      {
        if(polygon->contains(p))
        {
          if(gpoint.owner() == otherOwner)
          {
            polygon->setFilled(true);
            m_steps->addCaptured();
          }
            
          gpoint.capture();
          break;
        }
      }
    }
    
    for(Graph::iterator itr = graph.begin(), itrEnd = graph.end();
        itr != itrEnd; ++itr)
    {
      if(itr->isCaptured() || itr->owner() != Owner::NONE)
        continue;
      
      for(const Polygon_ptr& polygon : polyList)
      {
        const Point& newPoint = itr.point();

        if(polygon->contains(newPoint) && polygon->isFilled())
        {
          itr->capture();
          m_steps->addEmptyCaptured();
          break;
        }
      }  
    }
    
    drawPolygon(polyList);
    
    continueStep();
    emit nextPlayer(point);
  }
  
  void DotTable::continueStep()
  {
    const auto& allPoints = m_steps->getAllPoints();
    if(allPoints.size() + m_steps->emtyCapturedCount() == m_graph->width() * m_graph->height())
    {
      const int first = m_steps->getMarks(Owner::FIRST);
      const int second = m_steps->getMarks(Owner::SECOND);
      
      if(first > second)
        KMessageBox::information(0, i18n("The first player win!"), i18n("The first player win!"));
      else if(first < second)
        KMessageBox::information(0, i18n("The second player win!"), i18n("The second player win!"));
      else
        KMessageBox::information(0, i18n("Dead heat!"), i18n("Dead heat!"));
    }
    
    m_steps->nextStep();
  }

  //Hardcore undo process
  void DotTable::undo()
  {
    m_graph.reset(new Graph(m_config.m_width, m_config.m_height));
    m_polygons.clear();
    auto points(m_steps->getAllPoints());
    
    if(!points.empty())
      points.pop_back();
    m_steps->clear();

    for(const Point& point : points)
      pushPoint(point);
  }
  
  void DotTable::drawPolygon(PolyList polygons)
  {
    for(Polygon_ptr& polygon : polygons)
    {
      if(!polygon->isFilled())
        continue;
      
      polygon->setOwner(m_steps->getCurrentOwner());
      m_polygons.push_back(polygon);
      
      Point prevPoint = polygon->points().back();
      
      for(const Point& currPoint : polygon->points())
      {
        m_graph->addEdge(prevPoint, currPoint);
        prevPoint = currPoint;
      }
    }
  }

  const std::vector<Polygon_ptr>& DotTable::polygons() const
  {
    return m_polygons;
  }

  const Graph& DotTable::graph() const
  {
    return *m_graph;
  }

  const StepQueue& DotTable::stepQueue() const
  {
    return *m_steps;
  }
}

#include "dottable.moc"
