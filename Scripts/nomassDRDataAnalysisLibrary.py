"""

Created by Ana Sancho (user ezxas5)
on 16 Feb 2017 at 10:56.

This script is intended to read simulation output files, extract the necessary information, and
write useful files to plot.

"""

import numpy as np
import pandas as pd

## Quick analysis of Received and Supplied
def powerReceivedQuickAnalysis(resultsDF, configInfoDF):
    columns = pd.MultiIndex([[],[]],[[],[]],names=['building', 'col'])
    dataframe= pd.DataFrame(columns = columns)

    for buildID, building in configInfoDF.iterrows():
#         print buildID, building.LargeApplianceList
        large_grid_list =  ['Building%i_Appliance%i_received_grid' %(buildID,appID) for appID in building.LargeApplianceList]
        large_neigh_list =  ['Building%i_Appliance%i_received_neighbourhood' %(buildID,appID) for appID in building.LargeApplianceList]
        large_local_list =  ['Building%i_Appliance%i_received_local' %(buildID,appID) for appID in building.LargeApplianceList]
        large_general_list = ['Building%i_Appliance%i_received' %(buildID,appID) for appID in building.LargeApplianceList]

        small_grid_list =  ['Building%i_Appliance%i_received_grid' %(buildID,appID) for appID in building.SmallApplianceList]
        small_neigh_list =  ['Building%i_Appliance%i_received_neighbourhood' %(buildID,appID) for appID in building.SmallApplianceList]
        small_local_list =  ['Building%i_Appliance%i_received_local' %(buildID,appID) for appID in building.SmallApplianceList]
        small_general_list =  ['Building%i_Appliance%i_received' %(buildID,appID) for appID in building.SmallApplianceList]

        dataframe[buildID,'received_grid'] = resultsDF.loc[:,small_grid_list].sum(axis=1)   + resultsDF.loc[:,large_grid_list].sum(axis=1)
        dataframe[buildID,'received_neigh'] = resultsDF.loc[:,small_neigh_list].sum(axis=1) + resultsDF.loc[:,large_neigh_list].sum(axis=1)
        dataframe[buildID,'received_local'] = resultsDF.loc[:,small_local_list].sum(axis=1) + resultsDF.loc[:,large_local_list].sum(axis=1)
        dataframe[buildID,'received_general'] = resultsDF.loc[:,small_general_list].sum(axis=1) + resultsDF.loc[:,large_general_list].sum(axis=1)

    return dataframe

def powerSuppliedQuickAnalysis(resultsDF, configInfoDF):
    columns = pd.MultiIndex([[],[]],[[],[]],names=['building', 'col'])
    dataframe= pd.DataFrame(columns = columns)

    for buildID, building in configInfoDF.iterrows():
#         print buildID, building.PVList
        pv_grid_list =  ['Building%i_Appliance%i_supplied_grid' %(buildID,appID) for appID in building.PVList]
        pv_neigh_list =  ['Building%i_Appliance%i_supplied_neighbourhood' %(buildID,appID) for appID in building.PVList]
        pv_local_list =  ['Building%i_Appliance%i_supplied_local' %(buildID,appID) for appID in building.PVList]
        pv_general_list = ['Building%i_Appliance%i_supplied' %(buildID,appID) for appID in building.PVList]

        dataframe[buildID,'supplied_grid'] =  resultsDF.loc[:,pv_grid_list].sum(axis=1)
        dataframe[buildID,'supplied_neigh'] =  resultsDF.loc[:,pv_neigh_list].sum(axis=1)
        dataframe[buildID,'supplied_local'] = resultsDF.loc[:,pv_local_list].sum(axis=1)
        dataframe[buildID,'supplied_general'] =  resultsDF.loc[:,pv_general_list].sum(axis=1)

    return dataframe

def powerBatteriesSuppliedQuickAnalysis(resultsDF, configInfoDF):
    columns = pd.MultiIndex([[],[]],[[],[]],names=['building', 'col'])
    dataframe= pd.DataFrame(columns = columns)

    for buildID, building in configInfoDF.iterrows():
#         print buildID, building.PVList
        bat_grid_list =  ['Building%i_Appliance%i_supplied_grid' %(buildID,appID) for appID in building.BatteryList]
        bat_neigh_list =  ['Building%i_Appliance%i_supplied_neighbourhood' %(buildID,appID) for appID in building.BatteryList]
        bat_local_list =  ['Building%i_Appliance%i_supplied_local' %(buildID,appID) for appID in building.BatteryList]
        bat_general_list = ['Building%i_Appliance%i_supplied' %(buildID,appID) for appID in building.BatteryList]

        dataframe[buildID,'supplied_grid'] =  resultsDF.loc[:,bat_grid_list].sum(axis=1)
        dataframe[buildID,'supplied_neigh'] =  resultsDF.loc[:,bat_neigh_list].sum(axis=1)
        dataframe[buildID,'supplied_local'] = resultsDF.loc[:,bat_local_list].sum(axis=1)
        dataframe[buildID,'supplied_general'] =  resultsDF.loc[:,bat_general_list].sum(axis=1)

    return dataframe

def powerBatteriesReceivedQuickAnalysis(resultsDF, configInfoDF):
    columns = pd.MultiIndex([[],[]],[[],[]],names=['building', 'col'])
    dataframe= pd.DataFrame(columns = columns)

    for buildID, building in configInfoDF.iterrows():
#         print buildID, building.PVList
        bat_grid_list =  ['Building%i_Appliance%i_received_grid' %(buildID,appID) for appID in building.BatteryList]
        bat_neigh_list =  ['Building%i_Appliance%i_received_neighbourhood' %(buildID,appID) for appID in building.BatteryList]
        bat_local_list =  ['Building%i_Appliance%i_received_local' %(buildID,appID) for appID in building.BatteryList]
        bat_general_list = ['Building%i_Appliance%i_received' %(buildID,appID) for appID in building.BatteryList]

        dataframe[buildID,'received_grid'] =  resultsDF.loc[:,bat_grid_list].sum(axis=1)
        dataframe[buildID,'received_neigh'] =  resultsDF.loc[:,bat_neigh_list].sum(axis=1)
        dataframe[buildID,'received_local'] = resultsDF.loc[:,bat_local_list].sum(axis=1)
        dataframe[buildID,'received_general'] =  resultsDF.loc[:,bat_general_list].sum(axis=1)

    return dataframe



# def analysisNeighbourhood(dataframe, tariff_nparray, configInfoDF):
#
# def analysisLocal(dataframe, tariff_nparray, configInfoDF):
#     """
#     For each building analyse demand and supply, where it comes from.
#     :param dataframe:
#     :param tariff_nparray:
#     :param configInfoDF:
#     :return:
#    """



class columns(object):
    """
    Define columns of use for the output files.
    """
    def __init__(self, configInfoDF, buildingID):
        # Info cols
        self.columns_info   = [ 'nsim',
                                'TimeStep',
                                'hour',
                                'day',
                                'hourOfDay',
                                'minuteOfDay']
                                # 'month']

        # list of devices in building
        self.LargeApplianceList = configInfoDF.loc[buildingID].LargeApplianceList
        self.SmallApplianceList = configInfoDF.loc[buildingID].SmallApplianceList
        self.ShiftApplianceList = configInfoDF.loc[buildingID].ShiftApplianceList
        self.PVList             = configInfoDF.loc[buildingID].PVList
        self.BatteryList        = configInfoDF.loc[buildingID].BatteryList
        self.csvList            = configInfoDF.loc[buildingID].csvList


            # received_local (from pv and battery)
        self.columns_large_received_local = ['Building%i_Appliance%i_received_local'%(buildingID, appID) for appID in self.LargeApplianceList]
        self.columns_shift_received_local = ['Building%i_Appliance%i_received_local'%(buildingID, appID) for appID in self.ShiftApplianceList]
        self.columns_small_received_local = ['Building%i_Appliance%i_received_local'%(buildingID, appID) for appID in self.SmallApplianceList]
        self.columns_csv_received_local   = ['Building%i_Appliance%i_received_local'%(buildingID, appID) for appID in self.csvList]
        self.columns_battery_received_local=['Building%i_Appliance%i_received_local'%(buildingID, appID) for appID in self.BatteryList]

        self.columns_received_local_appliances = self.columns_large_received_local + \
                                                 self.columns_shift_received_local + \
                                                 self.columns_small_received_local + \
                                                 self.columns_csv_received_local
        self.columns_received_local       =      self.columns_received_local_appliances  + self.columns_battery_received_local

        # supplied by the PV
        self.columns_pv_supplied_local     = ['Building%i_Appliance%i_supplied_local'%(buildingID, appID) for appID in self.PVList]
        self.columns_pv_supplied_grid      = ['Building%i_Appliance%i_supplied_grid' %(buildingID, appID) for appID in self.PVList]

        # supplied by the battery local and neighbourhood
        self.columns_battery_supplied_local= ['Building%i_Appliance%i_supplied_local'%(buildingID, appID) for appID in self.BatteryList]
        self.columns_battery_supplied_neigh =['Building%i_Appliance%i_supplied_neighbourhood'%(buildingID, appID) for appID in self.BatteryList]


        # received in appliances from the neighbourhood
        self.columns_large_received_neigh  = ['Building%i_Appliance%i_received_neighbourhood'%(buildingID, appID) for appID in self.LargeApplianceList]
        self.columns_shift_received_neigh  = ['Building%i_Appliance%i_received_neighbourhood'%(buildingID, appID) for appID in self.ShiftApplianceList]
        self.columns_small_received_neigh  = ['Building%i_Appliance%i_received_neighbourhood'%(buildingID, appID) for appID in self.SmallApplianceList]
        self.columns_csv_received_neigh    = ['Building%i_Appliance%i_received_neighbourhood'%(buildingID, appID) for appID in self.csvList]
        self.columns_battery_received_neigh=['Building%i_Appliance%i_received_neighbourhood'%(buildingID, appID) for appID in self.BatteryList]

        self.columns_received_neigh_appliances = self.columns_large_received_neigh + \
                                                 self.columns_shift_received_neigh + \
                                                 self.columns_small_received_neigh + \
                                                 self.columns_csv_received_neigh
        self.columns_received_neigh       =      self.columns_received_neigh_appliances  + self.columns_battery_received_neigh

        # received from the grid
        self.columns_large_received_grid  = ['Building%i_Appliance%i_received_grid'%(buildingID, appID) for appID in self.LargeApplianceList]
        self.columns_shift_received_grid  = ['Building%i_Appliance%i_received_grid'%(buildingID, appID) for appID in self.ShiftApplianceList]
        self.columns_small_received_grid  = ['Building%i_Appliance%i_received_grid'%(buildingID, appID) for appID in self.SmallApplianceList]
        self.columns_csv_received_grid    = ['Building%i_Appliance%i_received_grid'%(buildingID, appID) for appID in self.csvList]
        self.columns_received_grid        = self.columns_large_received_grid + \
                                            self.columns_shift_received_grid + \
                                            self.columns_small_received_grid + \
                                            self.columns_csv_received_grid
        # to get TOTAL power from grid:
        self.grid_power = ['grid_power']


        # totals
        self.column_large_appliance_sum_received    = ['Building%i_Appliance_Large_Sum_received'%buildingID for k in range(bool(self.LargeApplianceList))]
        self.column_shift_appliance_sum_received    = ['Building%i_Appliance_LargeLearning_Sum_received'%buildingID for k in range(bool(self.ShiftApplianceList))]
        self.column_small_appliance_sum_received    = ['Building%i_Appliance_Small_Sum_received'%buildingID for k in range(bool(self.SmallApplianceList))]
        # self.column_nonshift_appliance_sum_received = ['Building%i_Appliance_NonShift_Sum_received'%buildingID] #large+small+csv+FMI
        self.column_csv_appliance_sum_received      = ['Building%i_Appliance_CSV_Sum_received'%buildingID for k in range(bool(self.csvList))]
        self.columns_battery_sum_received           = ['Building%i_Appliance_Battery_Sum_received'%buildingID for k in range(bool(self.BatteryList))]




        #  #  Cost
        # self.columns_cost_shift    =   ['Building%i_Appliance1_cost'%buildingID,     # washing machine
        #                                 'Building%i_Appliance4_cost'%buildingID]     # dishwasher
        # self.columns_cost_nonshift =   ['Building%i_Appliance0_cost'%buildingID,     # TV
        #                                 'Building%i_Appliance2_cost'%buildingID,     # microwave
        #                                 'Building%i_Appliance3_cost'%buildingID,     # cooker
        #                                 'Building%i_Appliance5_cost'%buildingID,     # fridge
        #                                 'Building%i_Appliance6_cost'%buildingID,     # audio-visual
        #                                 'Building%i_Appliance7_cost'%buildingID,     # other
        #                                 'Building%i_Appliance8_cost'%buildingID,     # computing
        #                                 'Building%i_Appliance9_cost'%buildingID,     # kitchen
        #                                 'Building%i_Appliance11_cost'%buildingID]    # heating
        # self.columns_cost_supply   =   ['Building%i_Appliance10_cost'%buildingID]    # PV
        # self.columns_cost_battery  =   ['Building%i_Appliance12_cost'%buildingID] # battery
        # self.columns_cost_grid     =   ['grid_cost']
        #
        # self.columns_to_keep =    self.columns_info            + self.columns_demand_shift    + \
        #                           self.columns_demand_nonshift + self.columns_supply_source   + \
        #                           self.columns_supply_grid     + self.columns_cost_shift      + \
        #                           self.columns_cost_nonshift   + self.columns_cost_supply     + \
        #                           self.columns_cost_grid       + self.columns_shift_onEvents  + \
        #                           self.columns_demand_battery   + self.columns_supply_battery  + \
        #                           self.columns_cost_battery


def replaceProfiles(dataframe, cycles_csv):
    """
    replace profiles of dishwasher and washing machine, for the moment.
    :param incorrectColumn:
    :param cycle:
    :return:
    """
    def replace(app_number, cycle_array, colName):
        incorrectColumn = dataframe[colName]
        length_cycle = len(cycle_array) - 1
        flag = incorrectColumn != 0
        starting = (pd.Series(incorrectColumn !=0) & pd.Series(flag!=flag.shift(1)))
        ind =starting[starting].index
        for index in ind:
            incorrectColumn.loc[index:(index+length_cycle)]=cycle_array
    #1 is WM.
    replace(1, cycles_csv.washing_machine.dropna().values, 'Building%i_Appliance1_received')
    # 4 is DW
    replace(4, cycles_csv.dishwasher.dropna().values, 'Building%i_Appliance4_received')




def analysisDF(dataframe, tariff_nparray):
    """
    Define columns of use for the output files.
        - Power in/out
        - Startin time
        - Cost
    """
    keepColumns = columns()
    dataframe = dataframe.loc[:,keepColumns.columns_to_keep]

    dataframe['demand_old_shift'] = dataframe.loc[:, keepColumns.columns_demand_shift].sum(axis=1)
    # substitute the shift columns with the data
    cycles_csv = pd.read_csv('cycles.csv')  # This file should be on the Scripts folder.
    replaceProfiles(dataframe, cycles_csv)  # Replace the simulated profiles with real data

    # demand and supply
    dataframe['demand_shift']               = dataframe.loc[:, keepColumns.columns_demand_shift].sum(axis=1)
    dataframe['demand_non_shift']           = dataframe.loc[:, keepColumns.columns_demand_nonshift].sum(axis=1)
    dataframe['demand_appliances']          = dataframe['demand_shift'] + dataframe['demand_non_shift']
    dataframe['demand_battery']             = dataframe.loc[:, keepColumns.columns_demand_battery].sum(axis=1)
    dataframe['demand_total']               = dataframe['demand_appliances'] + dataframe['demand_battery']

    dataframe['supply_sources']             = dataframe.loc[:, keepColumns.columns_supply_source].sum(axis=1)
    dataframe['supply_grid']                = dataframe.loc[:, keepColumns.columns_supply_grid].sum(axis=1) # imported from grid
    dataframe['supply_battery']             = dataframe.loc[:, keepColumns.columns_supply_battery].sum(axis=1)
    # dataframe['supply_total']               = dataframe['supply_sources'] + dataframe['supply_battery']

    # modify supply, after modifying the shift demand (from models to data)
    dataframe.loc[:,'difference']           =  dataframe.demand_shift - dataframe.demand_old_shift
    dataframe.loc[:,'surplus']              = dataframe.supply_sources - dataframe.demand_total
    dataframe.loc[dataframe.surplus <0, 'surplus'] = 0
    dataframe.loc[dataframe.surplus > dataframe.difference, 'delta_pv']   = dataframe.difference
    dataframe.loc[dataframe.surplus < dataframe.difference, 'delta_pv']   = dataframe.surplus
    dataframe.loc[dataframe.surplus < dataframe.difference, 'delta_grid'] = dataframe.difference - dataframe.surplus
    dataframe.loc[:, ['delta_pv','delta_grid']] = dataframe[['delta_pv','delta_grid']].fillna(0)
    # dataframe.loc[:, 'supply_sources']      = dataframe.supply_sources + dataframe.delta_pv
    dataframe.loc[:, 'supply_grid']         = dataframe.supply_grid    + dataframe.delta_grid

    # intermediate columns
    dataframe['pvUse_appliances']          = dataframe['demand_appliances'] - dataframe['supply_grid'] - dataframe['supply_battery']
    dataframe['pvUse_battery']             = dataframe['demand_battery']
    dataframe['pvUse']                     = dataframe['pvUse_appliances'] + dataframe['pvUse_battery']
    dataframe['pvExport']                  = dataframe['supply_sources']   - dataframe['pvUse']

    # off/on/mid peak given the slot tariff
    dic = {0.1: 'off-peak',0.5:'mid-peak', 0.9:'on-peak'}
    dataframe['peakTimeValue']             = tariff_nparray[dataframe.hourOfDay]
    dataframe['peakTime']                  = dataframe['peakTimeValue'].replace(dic, inplace=False)

    #cost
     # fix for shift appliances
    # dataframe['Building%i_Appliance1_cost'%buildingID] = dataframe['Building%i_Appliance1_received'] * dataframe['peakTimeValue']
    # dataframe['Building%i_Appliance4_cost'%buildingID] = dataframe['Building%i_Appliance4_received'] * dataframe['peakTimeValue']

    dataframe['cost_appliances_shift']     = dataframe['demand_shift'] * dataframe['peakTimeValue']
    dataframe['cost_appliances_non_shift'] = dataframe.loc[:, keepColumns.columns_cost_nonshift].sum(axis=1)
    dataframe['cost_appliances']           = dataframe['cost_appliances_shift'] + dataframe['cost_appliances_non_shift']
    dataframe['cost_battery']              = dataframe.loc[:, keepColumns.columns_cost_battery].sum(axis=1)
    dataframe['cost_total']                = dataframe['cost_appliances'] + dataframe['cost_battery']

    return dataframe


def weeklyDF(dataframe):
    grouped = dataframe.groupby('nsim')
    grouped2 = dataframe.groupby(['peakTime','nsim'])

    wdataframe = pd.DataFrame([])
    wdataframe['demand_appliances']       = grouped['demand_appliances'].sum()   # B+H+E
    wdataframe['demand_appliances_shift'] = grouped['demand_shift'].sum()  # B+H+E for shiftable
    wdataframe['demand']        = grouped['demand_total'].sum()                   # B+H+E+C
    wdataframe['supply_pv']     = grouped['supply_sources'].sum()                 # A
    wdataframe['supply_grid']   = grouped['supply_grid'].sum()                    # F
    wdataframe['pvExport']      = grouped['pvExport'].sum()                       # J
    wdataframe['pvUse']         = grouped['pvUse'].sum()                          #B+E

    wdataframe['self_cons_index']     = wdataframe['pvUse']/ wdataframe['demand'] *100
    wdataframe['self_cons_potential'] = wdataframe['supply_pv']   / wdataframe['demand'] *100

    # time of use grid import:
    wdataframe = wdataframe.join(grouped2['supply_grid'].sum().unstack(level=0)) # add the grid import for on/off/mid peak.

    # cost
    wdataframe['cost_appliances_shift']     = grouped['cost_appliances_shift'].sum()
    wdataframe['cost_appliances_non_shift'] = grouped['cost_appliances_non_shift'].sum()
    wdataframe['cost_appliances']           = grouped['cost_appliances'].sum()
    wdataframe['cost_battery']              = grouped['cost_battery'].sum()
    wdataframe['cost_total']                = grouped['cost_total'].sum()

    return wdataframe


def building_dataframe(dataframe, building, configInfoDF, tariff_nparray = None):
    bd_columns = columns(configInfoDF, building)
    building_dataframe = dataframe[bd_columns.columns_info]

    building_dataframe.loc[:,'demand_large'] = dataframe[bd_columns.column_large_appliance_sum_received].sum(axis=1) #sum doesnt do anything for calculation, but solves problem of empty columns
    building_dataframe.loc[:,'demand_shift'] = dataframe[bd_columns.column_shift_appliance_sum_received].sum(axis=1) #sum doesnt do anything for calculation, but solves problem of empty columns
    building_dataframe.loc[:,'demand_small'] = dataframe[bd_columns.column_small_appliance_sum_received].sum(axis=1) #sum doesnt do anything for calculation, but solves problem of empty columns
    building_dataframe.loc[:,'demand_csv']   = dataframe[bd_columns.column_csv_appliance_sum_received].sum(axis=1) #sum doesnt do anything for calculation, but solves problem of empty columns
    building_dataframe.loc[:,'demand_all']   =   building_dataframe['demand_large'] + \
                                                 building_dataframe['demand_shift'] + \
                                                 building_dataframe['demand_small'] + \
                                                 building_dataframe['demand_csv']

    building_dataframe.loc[:,'battery_demand_local'] =  dataframe[bd_columns.columns_battery_received_local].sum(axis=1)  #B in diagram
    building_dataframe.loc[:,'battery_supply_local'] = dataframe[bd_columns.columns_battery_supplied_local].sum(axis=1)   #C in the diagram

    building_dataframe.loc[:,'pv_supplied_local']    = dataframe[bd_columns.columns_pv_supplied_local].sum(axis=1)          #A+B
    building_dataframe.loc[:,'pv_toAppliances_local']= building_dataframe['pv_supplied_local']  - building_dataframe['battery_demand_local'] # = A in the diagram
    building_dataframe.loc[:,'pv_use_local']         =  building_dataframe['pv_toAppliances_local']  + building_dataframe['battery_supply_local'] #A+C

    #neighbourhood
    building_dataframe.loc[:,'pv_use_neigh']         = dataframe[bd_columns.columns_received_neigh_appliances].sum(axis=1) # E1 + E2
    building_dataframe.loc[:,'battery_supply_neigh'] = dataframe[bd_columns.columns_battery_supplied_neigh].sum(axis=1) # G
    building_dataframe.loc[:,'battery_demand_neigh'] = dataframe[bd_columns.columns_battery_received_neigh].sum(axis=1) # H

    # global
    building_dataframe.loc[:,'pv_export_grid']       = dataframe[bd_columns.columns_pv_supplied_grid].sum(axis=1)   # J
    building_dataframe.loc[:,'grid_import_local']    = dataframe[bd_columns.columns_received_grid].sum(axis=1)      # D
    building_dataframe.loc[:,'grid_import_global']   = dataframe['grid_power']   # this value can be accessed from any building, and it corresponds to the whole neighbourhood


    # off/on/mid peak given the slot tariff
    if tariff_nparray is not None:
        dic = {0.1: 'off-peak',0.5:'mid-peak', 0.9:'on-peak'}
        building_dataframe.loc[:,'peakTimeValue']    = tariff_nparray[building_dataframe.hourOfDay.astype(int)]
        building_dataframe.loc[:,'peakTime']         = building_dataframe['peakTimeValue'].replace(dic, inplace=False)

    return building_dataframe



def building_weeklyDF(building_dataframe, tariff_nparray = None):
    grouped = building_dataframe.groupby('nsim')
    grouped2 = building_dataframe.groupby(['peakTime','nsim'])

    wdataframe = pd.DataFrame([])                                               #new categories in page 20/02/17 of lab notebook
    wdataframe['demand_shift']         = grouped['demand_shift'].sum()          # A+C+E+D for shiftable
    wdataframe['demand_all']           = grouped['demand_all'].sum()            # A+C+E+D
    wdataframe['pv_supplied_local']    = grouped['pv_supplied_local'].sum()     # A + B
    wdataframe['pv_toAppliances_local']= grouped['pv_toAppliances_local'].sum() # A

    # local self consumption
    wdataframe['pv_use_local']         = grouped['pv_use_local'].sum()          # A + C
    wdataframe['battery_demand_local'] = grouped['battery_demand_local'].sum()  # B
    wdataframe['battery_supply_local'] = grouped['battery_supply_local'].sum()  # C

    wdataframe['self_cons_local']      = wdataframe['pv_use_local']/ wdataframe['demand_all'] *100  # (A+C)/(A+C+D+E)
    wdataframe['self_cons_potential_local']  = wdataframe['pv_supplied_local'] / wdataframe['demand_all'] *100

    # self consumption in community
    wdataframe['pv_use_neigh']         = grouped['pv_use_neigh'].sum()           # E
    wdataframe['self_cons_neigh']      = (wdataframe['pv_use_local'] + wdataframe['pv_use_neigh']) / \
                                          wdataframe['demand_all'] *100  # (A+C+E1+E2)/(A+C+D+E!+E2anal)

    # exports:
    wdataframe['pvExport'] = grouped['pv_export_grid'].sum()
    # time of use grid import:
    if tariff_nparray is not None:
         wdataframe = wdataframe.join(grouped2['grid_import_local'].sum().unstack(level=0)) # add the grid import for on/off/mid peak.

    # # cost
    # wdataframe['cost_appliances_shift']     = grouped['cost_appliances_shift'].sum()
    # wdataframe['cost_appliances_non_shift'] = grouped['cost_appliances_non_shift'].sum()
    # wdataframe['cost_appliances']           = grouped['cost_appliances'].sum()
    # wdataframe['cost_battery']              = grouped['cost_battery'].sum()
    # wdataframe['cost_total']                = grouped['cost_total'].sum()

    return wdataframe

def analysis_SC_local(dataframe, configInfoDF, tariff_nparray = None):
    cols = pd.MultiIndex([[],[]],[[],[]],names=['building', 'col'])
    localSCdataframe = pd.DataFrame(columns = cols)

    for buildID, building in configInfoDF.iterrows():
        building_wdf = building_weeklyDF(building_dataframe(dataframe, buildID, configInfoDF,tariff_nparray),tariff_nparray)
        # print building_wdf[['self_cons_local','self_cons_neigh','demand_all']].mean()


        localSCdataframe[buildID,'demand_all']           = building_wdf['demand_all']
        localSCdataframe[buildID,'pv_toAppliance_local'] = building_wdf['pv_toAppliances_local']
        localSCdataframe[buildID,'battery_supply_local'] = building_wdf['battery_supply_local']
        # localSCdataframe[buildID,'grid_supply_local'] =5
        localSCdataframe[buildID,'supply_neighbourhood'] = building_wdf['pv_use_neigh'] # REVISE
        localSCdataframe[buildID,'self_cons_local']      = building_wdf['self_cons_local']
        localSCdataframe[buildID,'self_cons_neigh']      = building_wdf['self_cons_neigh']

        if tariff_nparray is not None:
            localSCdataframe[buildID,'on-peak']       = building_wdf['on-peak']
            localSCdataframe[buildID,'mid-peak']      = building_wdf['mid-peak']
            localSCdataframe[buildID,'off-peak']      = building_wdf['off-peak']
            localSCdataframe[buildID,'pvExport']      = building_wdf['pvExport']


    return localSCdataframe

def analysis_SC_tocompare(SC_dataframe, number_pv, number_batt, number_buildings = 6.):
    SC_tocompare = SC_dataframe.unstack('building').unstack('col')[['self_cons_local','self_cons_neigh']]
    SC_tocompare['penetration'] = round(number_pv/number_buildings, 4)*100
    SC_tocompare['penetration_storage'] = number_batt/number_buildings*100

    SC_tocompare.reset_index(inplace=True,drop=False)
    SC_tocompare.rename(columns={'self_cons_local':'local','self_cons_neigh':'neighbourhood'}, inplace=True)
    SC_tocompare_final = pd.melt(SC_tocompare, id_vars = ['nsim','building','penetration', 'penetration_storage'],
                        value_vars=['local', 'neighbourhood'], var_name = 'Level',value_name='SC')
    return SC_tocompare_final

