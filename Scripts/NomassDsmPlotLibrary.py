"""

Created by Ana Sancho (user ezzas1) 
on 10 Nov 2016 at 09:25.

"""
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np


flatui = ["#e74c3c", "#34495e", "#2ecc71","#9b59b6", "#3498db", "#95a5a6",
          '#d95f0e', '#fec44f', '#1c9099','#005a32', '#f768a1','#a65628']

def DistributionStartingTimes(dataframe, appliance, building = 0, ylim=None):
    dic={1: 'washing-machine', 4:'dishwasher'}
    dataframe = dataframe[['Building%i_Appliance%i_previous_state'%(building, appliance),
                           'Building%i_Appliance%i_action'%(building, appliance),
                           'Building%i_Appliance10_supplied'%building]]
    dataframe = dataframe.dropna().reset_index(drop=True)
    previous_state_col = dataframe['Building%i_Appliance%i_previous_state'%(building, appliance)]
    shifted_state_col  = dataframe['Building%i_Appliance%i_action'%(building, appliance)]

    figure,axn = plt.subplots(1,2,  figsize = (8,3))
    sns.distplot(previous_state_col, kde=False, bins = 24, ax = axn[0])
    second = sns.distplot(shifted_state_col, kde=False, bins = 24, ax = axn[1])

    axn[0].set_ylabel('number of cases',fontsize = 16)
    axn[0].set_xlabel('Starting time without DR (h)',fontsize = 12)
    axn[1].set_xlabel('Starting time with DR (h)',fontsize = 12)
    axn[0].set_xlim([0,24])
    axn[1].set_xlim([0,24])
    if not ylim:  ylim = max(axn[0].get_ylim()[1], axn[1].get_ylim()[1])
    axn[0].set_ylim([0,ylim])
    axn[1].set_ylim([0,ylim])
    figure.suptitle('Disitribution of the starting times - %s'%dic[appliance],fontsize = 14)

    # for solar shadow
    # x= mean_pv.index/1440.*24
    # y=mean_pv.values
    # ax2 = axn[1].twinx()
    # # ax2.fill_between(x,0,y, color= flatui[7], alpha=0.3)
    # ax2.set_yticks([])
    # sec.plot(mean_pv.values)

    return figure

def boxplotSC(dataframe_week):
    dataframe = dataframe_week[['self_cons_index','self_cons_potential']]
    figure = plt.figure(figsize = (6,2))
    sns.set(style='white')
    sns.set_context('talk')
    sns.boxplot(dataframe, color='white', linewidth=1.4, orient='h')
    plt.setp(figure.get_axes()[0].lines, color='k', alpha=0.7)
    labels = ['self consumption', 'self consumption potential']
    plt.xlabel('percentage (%)')
    figure.get_axes()[0].set_yticklabels(labels)
    return figure

def ShowMeTariff(tariffArray):
    sns.set_context('talk')
    sns.set_style('ticks')
    figure = plt.figure(figsize = (10,3))
    plt.bar(range(0,24),tariffArray, color = flatui[5],edgecolor=flatui[5], label='TOU')
    plt.xlabel('time (h)')
    plt.xlim([0,24])
    plt.ylim([0,1])
    plt.ylabel('grid cost')
    return figure

def barplot(dataframe_week, per_simulation = False):
    dataframe = dataframe_week[['on-peak', 'mid-peak','off-peak','pvExport']]/60000  # to convert to kWh

    figure = plt.figure()
    color = [flatui[0], flatui[1], flatui[2], flatui[7]]
    if per_simulation:
        dataframe.plot.bar(stacked = True, color=color)
    else:
        dataframe.mean().plot.bar(stacked = False, color=color, figsize = (4,5))
    plt.ylabel('Energy (kWh)')
    # plt.legend().set_visible(False)
    plt.xticks(rotation=0)
    return figure

def boxplotSC_local_neigh(analysis_SC_tocompare_df, title = None):
    # dataframe = analysis_SC_tocompare_df.unstack('building').unstack('col')[['self_cons_local','self_cons_neigh']]
    # dataframe = dataframe_week[['self_cons_index','self_cons_potential']]
    figure = plt.figure(figsize = (6,2))
    sns.set(style='white')
    sns.boxplot(data = analysis_SC_tocompare_df,y='penetration',x='SC',hue='Level', orient='h',
            linewidth=1.1, color = 'gray', fliersize = 3.5, )
    # plt.setp(figure.get_axes()[0].lines, color='k', alpha=0.7)
    plt.ylabel('PV penetration (%)')
    plt.xlabel('Self-consumption (%)')
    plt.legend(bbox_to_anchor=(1.32,1))
    plt.xlim([-0.3,80])
    # figure.get_axes()[0].set_yticklabels(labels)
    if title: plt.title(title)

    return figure

def boxplotSC_multibuilding(analysis_SC_tocompare_df, title = None):
    figure = plt.figure(figsize = (6,6))
    sns.set(style='white')
    sns.boxplot(data = analysis_SC_tocompare_df,y='Level',x='SC',hue='building', orient='h',
            linewidth=1.1, palette = flatui, fliersize = 3.5, )
    # plt.setp(figure.get_axes()[0].lines, color='k', alpha=0.7)
    plt.ylabel(' ')
    plt.xlabel('Self-consumption (%)')
    plt.legend(loc = 4,  title='Building') #bbox_to_anchor=(1.32,1),
    # plt.xlim([0,30])
    # figure.get_axes()[0].set_yticklabels(labels)
    if title: plt.title(title)

    return figure

def barplot_multibuilding(analysis_SC_df, stacked = True):
    dataframe = analysis_SC_df.unstack('building').unstack('col')[['on-peak', 'mid-peak','off-peak','pvExport']]/60000  # to convert to kWh

    figure = plt.figure()
    color = [flatui[0], flatui[1], flatui[2], flatui[7]]
    if stacked:
        dataframe.mean(level=0).plot.bar(stacked=True, color=color)
    else:
        dataframe.mean(level=0).plot.bar(stacked=False, color=color)

    plt.ylabel('Energy (kWh)')
    # plt.legend().set_visible(False)
    plt.xticks(rotation=0)
    plt.xlabel('Building')
    return figure

def quickAnalysisGraph(quickAnalysisDF, title,  label_global ='_received', ):
    rec_graph = (quickAnalysisDF.sum().unstack())/10**5
    fig, axn = plt.subplots(1,2, figsize = (12,3), sharey=True,)
    rec_graph.ix[:,:3].plot.bar(stacked=True, color=flatui,ax = axn[0], grid=True)
    rec_graph.ix[:,3].plot.bar(color=flatui[5],ax = axn[1], grid=True, label = label_global, legend=True,)
    axn[0].set_title(title)
    axn[0].set_ylabel(title +' power (x10^5)')
    return fig