/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
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
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "boardmodel.hpp"
#include "boardmodel_p.hpp"

#include <QDebug>

#include <KMessageBox>
#include <KLocalizedString>

#include <interface/irival.hpp>

#include "iboardview.hpp"
#include "graph.hpp"
#include "polygonfinder.hpp"
#include "stepqueue.hpp"

namespace KDots
{
  BoardModelPrivate::BoardModelPrivate(const GameConfig& config,
                                       std::unique_ptr<StepQueue>&& step_queue,
                                       BoardModel *parent)
    : QObject(parent)
    , m_graph(new Graph(config.m_width, config.m_height))
    , m_steps(std::move(step_queue))
    , m_config(config)
    , q_ptr(parent)
  {
  }
  
  void BoardModelPrivate::drawPolygon(PolyList polygons)
  {
    for (Polygon_ptr& polygon : polygons)
    {
      if (!polygon->isFilled())
        continue;

      polygon->setOwner(m_steps->getCurrentOwner());
      m_polygons.push_back(polygon);

      Point prevPoint = polygon->points().back();

      for (const Point& currPoint : polygon->points())
      {
        m_graph->addEdge(prevPoint, currPoint);
        prevPoint = currPoint;
      }
    }
  }
  
  namespace
  {
    QString getResult(int firstPoints, int secondPoints)
    {
      if (firstPoints > secondPoints)
        return i18n("The first player win!");

      if (firstPoints < secondPoints)
        return i18n("The second player win!");

      return i18n("Dead heat!");
    }
  }
  
  void BoardModelPrivate::continueStep()
  {
    const auto& allPoints = m_steps->getAllPoints();
    if (allPoints.size() + m_steps->emptyCapturedCount() == m_graph->width() * m_graph->height())
    {
      const int first = m_steps->getMarks(Owner::FIRST);
      const int second = m_steps->getMarks(Owner::SECOND);

      const QString& message = getResult(first, second);
      KMessageBox::information(0, message, message);
      return;
    }

    m_steps->nextStep();
  }
  
  void BoardModelPrivate::emitStatus()
  {
    const QString& firstMark = QString::number(m_steps->getMarks(Owner::FIRST));
    const QString& secondMark = QString::number(m_steps->getMarks(Owner::SECOND));
    
    Q_Q(BoardModel);
    emit q->statusUpdated(QString::fromAscii("First:\t%1\tSecond:\t%2").arg(firstMark, secondMark));
  }
  
  
  void BoardModelPrivate::addPoint(const Point& point)
  {
    Q_Q(BoardModel);
    
    if (sender() == m_rival.get())
    {
      if (m_rival->owner() != m_steps->getCurrentOwner())
        return;
    }
    else
    {
      if (m_rival->owner() == m_steps->getCurrentOwner())
        return;
    }

    Graph& graph = *m_graph;
    GraphPoint& currentPoint = graph[point];

    if (currentPoint.owner() != Owner::NONE || currentPoint.isCaptured())
      return;

    const Owner current = m_steps->getCurrentOwner();

    currentPoint.setOwner(current);

    m_steps->addPoint(point);

    PolygonFinder findPolygon(graph, current);

    //O(n)
    const PolyList& polyList = findPolygon(point);

    const auto& points = m_steps->getPoints(StepQueue::other(current));
    if (points.empty() || polyList.empty())
    {
      continueStep();
      emitStatus();
      emit q->pointAdded(point);
      return;
    }

    const Owner otherOwner = StepQueue::other(current);

    const auto& otherOwnerPoints = m_steps->getPoints(otherOwner);
    for (const Point& p : otherOwnerPoints)
    {
      GraphPoint& gpoint = graph[p];
      if (gpoint.isCaptured())
        continue;

      for (const Polygon_ptr& polygon : polyList)
      {
        if (polygon->contains(p))
        {
          if (gpoint.owner() == otherOwner)
          {
            polygon->setFilled(true);
            m_steps->addCaptured();
          }

          gpoint.capture();
          break;
        }
      }
    }

    for (Graph::iterator itr = graph.begin(), itrEnd = graph.end();
        itr != itrEnd; ++itr)
    {
      if (itr->isCaptured() || itr->owner() != Owner::NONE)
        continue;

      for (const Polygon_ptr& polygon : polyList)
      {
        const Point& newPoint = itr.point();

        if (polygon->contains(newPoint) && polygon->isFilled())
        {
          itr->capture();
          m_steps->addEmptyCaptured();
          break;
        }
      }
    }

    drawPolygon(polyList);

    continueStep();
    emitStatus();
    emit q->pointAdded(point);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  
  BoardModel::BoardModel(const GameConfig& config,
                         std::unique_ptr<StepQueue>&& step_queue,
                         QObject *parent)
    : QObject(parent)
    , d_ptr(new BoardModelPrivate(config, std::move(step_queue), this))
  {
  }
  
  BoardModel::~BoardModel()
  {
  }

  void BoardModel::setView(std::unique_ptr<IBoardView>&& view)
  {
    Q_D(BoardModel);
    d->m_view = std::move(view);
    d->m_view->setModel(this);

    connect(d->m_view.get(), SIGNAL(pointClicked(const Point&)),
            d, SLOT(addPoint(const Point&)));
  }

  void BoardModel::setRival(std::unique_ptr<IRival>&& rival)
  {
    Q_D(BoardModel);
    d->m_rival = std::move(rival);

    connect(this, SIGNAL(pointAdded(const Point&)),
            d->m_rival.get(), SLOT(onPointAdded(const Point&)));
    connect(d->m_rival.get(), SIGNAL(needAddPoint(const Point&)),
            d, SLOT(addPoint(const Point&)));

    d->m_rival->setBoardModel(this);
  }

  const GameConfig& BoardModel::gameConfig() const
  {
    Q_D(const BoardModel);
    return d->m_config;
  }

  //Hardcore undo process
  void BoardModel::undo()
  {
    Q_D(BoardModel);
    emit freezeView(true);
    d->m_graph.reset(new Graph(d->m_config.m_width, d->m_config.m_height));
    d->m_polygons.clear();
    auto points(d->m_steps->getAllPoints());

    if (!points.empty())
      points.pop_back();
    d->m_steps->clear();

    for (const Point& point : points)
      d->addPoint(point);

    emit freezeView(false);
  }

  const std::vector<Polygon_ptr>& BoardModel::polygons() const
  {
    Q_D(const BoardModel);
    return d->m_polygons;
  }

  const Graph& BoardModel::graph() const
  {
    Q_D(const BoardModel);
    return *d->m_graph;
  }

  const StepQueue& BoardModel::stepQueue() const
  {
    Q_D(const BoardModel);
    return *d->m_steps;
  }
}

#include "moc_boardmodel.cpp"
#include "moc_boardmodel_p.cpp"
