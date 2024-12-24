import time

station_names_A = [
    '정문', '약학대학', '해양대1호관', '본관',
    '학생회관', '인문대서쪽', '학생생활관', '인문대동쪽',
    '중앙도서관', '의과대학', '공대4호관', '교양동']

station_names_B = ['정문', '약학대학', '해양대1호관', '교양동' ,
                   '공대4호관', '의과대학', '중앙도서관', '인문대동쪽' ,
                   '학생생활관' , '인문대서쪽' ,'학생회관' , '본관' ]

#정문 기준 시간표 분으로 표현
station_arrival_time_roots = [
    [   #A 노선 시간표
        485, 505, 525, 570,
        605, 625, 645, 680,
        760, 785, 805, 825,
        870, 905, 925, 945,
        990, 1020, 1040, 1060,
        1080, 1100, 1120
    ],
    [   #B 노선 시간표 
        490, 510, 530, 580,
        610, 630, 650, 690,
        770, 790, 810, 830,
        880, 910, 930, 950,
        1000, 1030, 1050, 1070,
        1090, 1110, 1130
    ],
]

#노선 번호 대신 다른 숫자 입력시 무한 루프
while True:
    direct = int(input('1 : A노선, 2 : B노선\n노선숫자를 입력하세요: '))
    if direct == 1 or direct == 2:
        break
selected_arrival_time = station_arrival_time_roots[direct-1]

if direct == 1:
    guide_msg = '''[1.정문, 2.약학대학, 3.해양대1호관, 4.본관\n
 5.학생회관, 6.인문대서쪽, 7.학생생활관, 8.인문대동쪽\n
 9.중앙도서관, 10.의과대학, 11.공대4호관, 12.교양동]\n\n
 (띄어쓰기 하지마세요) 정류장(번호)을 입력하세요: '''
    select_station= station_names_A
else:
    guide_msg = '''[1.정문, 2.약학대학, 3.해양대1호관, 4.교양동\n
 5.공대4호관, 6.의과대학, 7.중앙도서관, 8.인문대동쪽\n
 9.학생생활관, 10.인문대서쪽, 11.학생회관, 12.본관]\n\n
(띄어쓰기 하지마세요) 정류장(번호)을 입력하세요: '''
    select_station= station_names_B
#정류장을 번호 또는 없는 이름 입력시  무한 루프
while True:    
    station_idx = input(guide_msg)
    if station_idx in select_station:
        station_idx=select_station.index(station_idx)+1
        break

    if station_idx.isdigit():
        station_idx= int(station_idx)
        if 0<station_idx <=12:
            break
    

# 상술한 정문 기준 시간표에 정류장 별 거리 계산해서 더해주는 역할.
selected_arrival_time = [x + station_idx-1 for x in selected_arrival_time]

current_time = time.localtime()
#현재 시간을 분으로 변환
time2min = current_time.tm_hour * 60 + current_time.tm_min


near_idx = 99

length = len(selected_arrival_time)
for i in range(length):
    if time2min > selected_arrival_time[i]:
        near_idx = i+1

if near_idx < length :
    arrival_time = selected_arrival_time[near_idx] - time2min
    print('현재 시간: ', time.strftime('%H시 %M분', current_time))
    
    if direct ==1 :
        print ('선택하신 노선은 A노선 입니다.')
        print(station_names_A[station_idx -1],'(의) 다음 버스는', int(selected_arrival_time[near_idx]/60),'시'
              , selected_arrival_time[near_idx]%60,'분 입니다.', arrival_time,'분 후에 도착합니다.')
        
      
    if direct ==2:
        print ('선택하신 노선은 B노선 입니다.')
        print(station_names_B[station_idx -1],'(의) 다음 버스는', int(selected_arrival_time[near_idx]/60),'시'
              , selected_arrival_time[near_idx]%60,'분 입니다.', arrival_time,'분 후에 도착합니다.')
   
else:
    print('막차가 지나갔습니다.')

